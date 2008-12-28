#include <string>
#include <sstream>

#include "DependentDOF.h"
#include "PrimitiveBase.h"

using namespace std;
using namespace GiNaC;


const string SQL_dependent_dof_database_schema = "CREATE TABLE dependent_dof_list (id INTEGER PRIMARY KEY, variable_name TEXT NOT NULL, expression TEXT NOT NULL, source_dof_table_name TEXT NOT NULL);";

DependentDOF :: DependentDOF (ex expression, std::vector<DOFPointer> source_dof_list):
DOF(false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

DependentDOF :: DependentDOF ( const char *name, ex expression, std::vector<DOFPointer> source_dof_list):
DOF(name,false /*free*/,true /*dependent*/)
{
	// @fixme Need to make sure that all DOF's in expression are included in the DOF list
	expression_ = expression;
	source_dof_list_ = source_dof_list;
}

double DependentDOF::GetValue()const
{
	// For each independent DOF in the list, substitute its value into the expression that defines this dependent DOF
	GiNaC::ex current_expression = GetExpression();
	double result;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(!source_dof_list_[current_dof]->IsDependent())
		{
			current_expression = current_expression.subs(source_dof_list_[current_dof]->GetVariable() ==
														 source_dof_list_[current_dof]->GetValue(),subs_options::no_pattern);
		}
	}

	// Now evaluate the expression to a numeric value
	// check to make sure the expression evaluates to a numeric value
	if (is_a<numeric>(current_expression)) {
		result = ex_to<numeric>(current_expression).to_double();
	} else {
		throw Ark3DException();
  	}

	return result;
}

GiNaC::ex DependentDOF::GetExpression()const
{
	// For each dependent DOF in the list, substitute its expression into the overall expression that defines this dependent DOF
	GiNaC::ex result = expression_;

	for(unsigned int current_dof = 0; current_dof < source_dof_list_.size(); current_dof++)
	{
		if(source_dof_list_[current_dof]->IsDependent())
		{
			result = result.subs(source_dof_list_[current_dof]->GetVariable() == source_dof_list_[current_dof]->GetExpression(),subs_options::no_pattern);
		}
	}

	return result;
}

// method for adding this object to the SQLite3 database
void DependentDOF::AddToDatabase(sqlite3 *database)
{
	// set the database for this object, in the future this database will be updated whenever this object is updated
	database_ = database;

	// First, create the sql statements to undo and redo this operation
	stringstream temp_stream;
	temp_stream.precision(__DBL_DIG__);
	temp_stream << "BEGIN; "
                << "INSERT INTO dependent_dof_list VALUES(" 
                << id_number_ << ",'" << variable_.get_name() << "','" 
				<< expression_ << "', 'source_dof_table_" << id_number_ <<"'); "
                << "INSERT INTO dof_list VALUES("
                << id_number_ << ",'dependent_dof_list'); "
                << "COMMIT; ";

	string sql_insert = temp_stream.str();

	temp_stream.str(""); // clears the string stream

	temp_stream << "BEGIN; "
				<< "DELETE FROM dof_list WHERE id=" << id_number_ 
				<< "; DELETE FROM dependent_dof_list WHERE id=" << id_number_ 
				<< "; COMMIT;";

	string sql_undo = temp_stream.str();

	// add this object to the appropriate tables by executing the SQL command sql_insert 
	char *zErrMsg = 0;
	int rc = sqlite3_exec(database_, sql_insert.c_str(), 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::cerr << "SQL error: " << zErrMsg << endl;
		sqlite3_free(zErrMsg);
		
		// the table "dependent_dof_list" may not exist, attempt to create
		rc = sqlite3_exec(database_, ("ROLLBACK;"+SQL_dependent_dof_database_schema).c_str(), 0, 0, &zErrMsg);  // need to add ROLLBACK since previous transaction failed
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
	// need to use sqlite3_mprintf to make sure the single quotes in the sql statements get escaped where needed
	char *sql_undo_redo = sqlite3_mprintf("INSERT INTO undo_redo_list(undo,redo) VALUES('%q','%q')",sql_undo.c_str(),sql_insert.c_str());

	rc = sqlite3_exec(database_, sql_undo_redo, 0, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		std::string error_description = "SQL error: " + std::string(zErrMsg);
		sqlite3_free(zErrMsg);
		throw Ark3DException(error_description);
	}

	sqlite3_free(sql_undo_redo);
}
