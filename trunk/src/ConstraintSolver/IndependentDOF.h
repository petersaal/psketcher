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

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);

	private:
		double value_;
};
typedef boost::shared_ptr<IndependentDOF> IndependentDOFPointer;


#endif //IndependentDOFH
