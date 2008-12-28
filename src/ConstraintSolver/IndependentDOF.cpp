#include <string>
#include <sstream>
#include "IndependentDOF.h"
#include "PrimitiveBase.h"

using namespace std;
using namespace GiNaC;

const std::string SQL_independent_dof_database_schema = "CREATE TABLE independent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, bool_free INTEGER NOT NULL, value REAL NOT NULL);";

IndependentDOF ::IndependentDOF ( double value, bool free):
DOF(free,false /*dependent*/)
{
	value_=value;
}

IndependentDOF :: IndependentDOF ( const char *name, double value, bool free):
DOF(name,free,false /*dependent*/)
{
	value_ = value;
}

// method for adding this object to the SQLite3 database
void IndependentDOF::AddToDatabase(sqlite3 *database)
{	
	// set the database for this object, in the future this database will be updated whenever this object is updated
	database_ = database;

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN;"
                << "INSERT INTO independent_dof_list VALUES(" 
                << id_number_ << ",'" << variable_.get_name() << "'," 
				<< free_ << "," << value_ <<");"
                << "INSERT INTO dof_list VALUES("
                << id_number_ << ",'independent_dof_list');"
                << "COMMIT;";

	string sql_insert = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN;"
				<< "DELETE FROM dof_list WHERE id=" << id_number_ 
				<< "DELETE FROM independent_dof_list WHERE id=" << id_number_ 
				<< "COMMIT;";

	string sql_undo = temp_stream.str();

	// add this object to the appropriate tables by executing the SQL command sql_insert 
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_insert.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		sqlite3_free(zErrMsg);
		
		// the table "independent_dof_list" may not exist, attempt to create
		rc = sqlite3_exec(database_, SQL_independent_dof_database_schema.c_str(), 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}

		// now that the table has been created, attempt the insert one last time
		rc = sqlite3_exec(database_, sql_insert.c_str(), 0, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
			std::string error_description = "SQL error: " + std::string(zErrMsg);
			sqlite3_free(zErrMsg);
			throw Ark3DException(error_description);
		}
	}

	// finally, update the undo_redo_list in the database with the database changes that have just been made
	temp_stream.str("");
	
	temp_stream << "INSERT INTO undo_redo_list(undo,redo) VALUES('"
				<< sql_undo << "','" << sql_insert << "');";

	rc = sqlite3_exec(database_, temp_stream.str().c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}
}
