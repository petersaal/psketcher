#include <sstream>

#include "PrimitiveBase.h"
#include "DependentDOF.h"

using namespace std;
using namespace GiNaC;

// Initialize private static variables for DOF and PrimitiveBase classes
unsigned PrimitiveBase::next_id_number_ = 1;

PrimitiveBase::PrimitiveBase():
id_number_(next_id_number_++),
selected_(false),
selectable_(true),
delete_me_(false),
database_(0)
{

}

void PrimitiveBase::SetSelectable(bool selectable)
{
	selectable_ = selectable;
	
	// if not selectable, need to make sure selected flag is false
	if(!selectable_)
		selected_ = false;
}

// Default selection mask implementation, each derived class should override this method
void PrimitiveBase::ApplySelectionMask(SelectionMask mask)
{
	if(mask == All)
		SetSelectable(true);
	else
		SetSelectable(false);
}

void PrimitiveBase::AddDOF(DOFPointer new_dof)
{
	// add the new dof to the list
	dof_list_.push_back(new_dof);

	// check to see if the dof to be added is a dependent, if so at the dof's that it dependends on as well
	if(dynamic_cast<DependentDOF*>(new_dof.get()) != 0)
	{
		DependentDOFPointer dependent_dof = boost::dynamic_pointer_cast<DependentDOF>(new_dof);
		
		std::vector<DOFPointer> temp_dof_list = dependent_dof->GetDOFList();

		for(unsigned int current_dof = 0; current_dof < temp_dof_list.size(); current_dof++)
			dof_list_.push_back(temp_dof_list[current_dof]);
	}
	
	// lastly, remove any duplicate degrees of freedom in the list
	sort( dof_list_.begin(), dof_list_.end());
	dof_list_.erase( unique( dof_list_.begin(), dof_list_.end()), dof_list_.end());
}

// returns true if deletion flag changes, otherwise returns false
bool PrimitiveBase::FlagForDeletionIfDependent(boost::shared_ptr<PrimitiveBase> input_primitive)
{
	// short circuit if already flagged for deletion
	if(IsFlaggedForDeletion())
		return false;

	// check to see if this primitive depends on the input primitive
	if (find(primitive_list_.begin(), primitive_list_.end(), input_primitive) != primitive_list_.end())
	{
		FlagForDeletion();
		return true;
	} else {
		return false;
	}
}

// utility method used to add the dof_list_ and the primitive_list_ to the database
void PrimitiveBase::AddListsToDatabase(const string &dof_list_table_name, const string &primitive_list_table_name)
{
	stringstream temp_stream;

	temp_stream << "BEGIN;"
				<< "CREATE TABLE " << dof_list_table_name << " (id INTEGER PRIMARY KEY);"
				<< "CREATE TABLE " << primitive_list_table_name << " (id INTEGER PRIMARY KEY);";

	// add the insert statements for each dof in dof_list_	
	for(unsigned int current_dof = 0; current_dof < dof_list_.size(); current_dof++)
	{
		temp_stream << "INSERT INTO " << dof_list_table_name << " VALUES(" << dof_list_[current_dof]->GetID() << "); ";
	}

	// add the insert statements for each primitive in primitive_list_	
	for(unsigned int current_primitive = 0; current_primitive < primitive_list_.size(); current_primitive++)
	{
		temp_stream << "INSERT INTO " << primitive_list_table_name << " VALUES(" << primitive_list_[current_primitive]->GetID() << "); ";
	}

	temp_stream << "COMMIT;";

	string sql_do = temp_stream.str();

	temp_stream.str("");

	// define the undo sql statement
	temp_stream << "BEGIN; "
 				<< "DROP TABLE " << dof_list_table_name
				<< "; DROP TABLE " << primitive_list_table_name
				<< "; COMMIT;";

	string sql_undo = temp_stream.str();

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