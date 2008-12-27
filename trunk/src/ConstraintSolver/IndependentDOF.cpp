#include "IndependentDOF.h"

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
	
}
