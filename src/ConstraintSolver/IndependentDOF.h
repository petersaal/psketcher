#ifndef IndependentDOFH
#define IndependentDOFH

#include "DOF.h"

// IndependentDOF class
class IndependentDOF : public DOF
{
	public:
		IndependentDOF ( double value = 0.0, bool free = false );
		IndependentDOF ( const char *name, double value = 0.0, bool free = false );

		//Accessor methods
		void SetValue ( double value );
		double GetValue()const {return value_;}
		virtual void SetFree(bool free);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddDelete(bool add_to_database); // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things

	private:
		double value_;
};
typedef boost::shared_ptr<IndependentDOF> IndependentDOFPointer;


#endif //IndependentDOFH
