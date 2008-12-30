#include <sstream>

#include "IndependentDOF.h"
#include "Point2D.h"

const std::string SQL_point2d_database_schema = "CREATE TABLE point2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, sketch_plane INTEGER NOT NULL, s_dof INTEGER NOT NULL, t_dof INTEGER NOT NULL);";

using namespace std;
using namespace GiNaC;

// Return the global coordinates for a Point2D object
void Point2D::Get3DLocation(double & x_location,double & y_location, double & z_location)const
{
	sketch_plane_->Get3DLocation(s_->GetValue(), t_->GetValue(), x_location, y_location, z_location);
}


Point2D :: Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free, bool t_free):
s_(new IndependentDOF(s,s_free)),
t_(new IndependentDOF(t,t_free)),
Primitive2DBase(sketch_plane)
{
	AddPrimitive(sketch_plane);

	AddDOF(s_);
	AddDOF(t_);
}

Point2D :: Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane):
s_(s),
t_(t),
Primitive2DBase(sketch_plane)
{
	AddPrimitive(sketch_plane);
	
	AddDOF(s_);
	AddDOF(t_);
}

void Point2D::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Points)
		SetSelectable(true);
	else
		SetSelectable(false);
}

mmcMatrix Point2D::GetmmcMatrix()const
{
	mmcMatrix result(2,1);
	result(0,0) = s_->GetValue();
	result(1,0) = t_->GetValue();

	return result;
}

// method for adding this object to the SQLite3 database, needs to be implement by each child class
void Point2D::AddToDatabase(sqlite3 *database)
{
	database_ = database;
	DatabaseAddRemove(true);
}

void Point2D::RemoveFromDatabase()
{
	DatabaseAddRemove(false);
}

void Point2D::DatabaseAddRemove(bool add_to_database) // Utility method used by AddToDatabase and RemoveFromDatabase
{
	string sql_do, sql_undo;

	stringstream dof_list_table_name;
	dof_list_table_name << "dof_table_" << GetID();
	stringstream primitive_list_table_name;
	primitive_list_table_name << "primitive_table_" << GetID();
	
	// "CREATE TABLE point2d_list (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, sketch_plane INTEGER NOT NULL, s_dof INTEGER NOT NULL, t_dof INTEGER NOT NULL);"

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO point2d_list VALUES(" 
                << GetID() << ",'" << dof_list_table_name.str() << "','" 
				<< primitive_list_table_name.str() << "'," << GetSketchPlane()->GetID() 
				<< "," << s_->GetID() << "," << t_->GetID() << "); "
                << "INSERT INTO primitive_list VALUES("
                << GetID() << ",'point2d_list'); "
                << "COMMIT; ";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM primitive_list WHERE id=" << GetID() 
				<< "; DELETE FROM point2d_list WHERE id=" << GetID() 
				<< "; COMMIT;";

	if(add_to_database)
		sql_undo = temp_stream.str();
	else
		sql_do = temp_stream.str();

	// add this object to the appropriate tables by executing the SQL command sql_insert 
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		if(add_to_database)
		{
			//std::cerr << "SQL error: " << zErrMsg << endl;
			sqlite3_free(zErrMsg);
			
			// the table "independent_dof_list" may not exist, attempt to create
			rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_point2d_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
	
			// now that the table has been created, attempt the insert one last time
			rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
			if( rc!=SQLITE_OK ){
				std::string error_description = "SQL error: " + std::string(zErrMsg);
				sqlite3_free(zErrMsg);
				throw Ark3DException(error_description);
			}
		} else {
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}
	}

	// finally, update the undo_redo_list in the database with the database changes that have just been made
	// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
	char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q')",sql_undo.c_str(),sql_do.c_str());

	rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	sqlite3_free(sql_undo_redo);

	// Now use the method provided by PrimitiveBase to create the tables listing the DOF's and the other Primitives that this primitive depends on
	DatabaseAddDeleteLists(add_to_database,dof_list_table_name.str(),primitive_list_table_name.str());
}