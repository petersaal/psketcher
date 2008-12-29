#include <sstream>

#include "ConstraintEquationBase.h"

using namespace std;
using namespace GiNaC;

void ConstraintEquationBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All || mask == Constraints)
		SetSelectable(true);
	else
		SetSelectable(false);
}

// Utility method to add the constraints_ list to the database
void ConstraintEquationBase::DatabaseAddDeleteConstraintList(bool add_to_database, const std::string &constraint_list_table_name)
{
	string sql_do, sql_undo;	
	stringstream temp_stream;

	temp_stream << "BEGIN;"
				<< "CREATE TABLE " << constraint_list_table_name << " (expression TEXT NOT NULL, weight FLOAT NOT NULL);";

	// add the insert statements for each constraint in constraints_	
	for(unsigned int current_constraint = 0; current_constraint < constraints_.size(); current_constraint++)
	{
		temp_stream << "INSERT INTO " << constraint_list_table_name << " VALUES('" << constraints_[current_constraint] << "'," << weight_list_[current_constraint] << "); ";
	}

	temp_stream << "COMMIT;";

	if(add_to_database)
		sql_do = temp_stream.str();
	else
		sql_undo = temp_stream.str();

	temp_stream.str("");

	// define the undo sql statement
	temp_stream << "BEGIN; "
 				<< "DROP TABLE " << constraint_list_table_name
				<< "; COMMIT;";

	if(add_to_database)
		sql_undo = temp_stream.str();
	else
		sql_do = temp_stream.str();

	// go ahead and create the sql tables
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_do.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
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
}