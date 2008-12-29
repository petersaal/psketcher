#ifndef DOFH
#define DOFH

#include <boost/shared_ptr.hpp>
#include <ginac/ginac.h>
#include <sqlite3.h>

// Abstract DOF base class
class DOF
{
	public:
		DOF (bool free, bool dependent);
		DOF ( const char *name, bool free, bool dependent);

		virtual ~DOF() {;}

		//Accessor methods
		virtual void SetValue ( double value ) = 0;
		virtual double GetValue()const = 0;
		virtual GiNaC::ex GetExpression()const {return variable_;}

		unsigned GetID()const {return id_number_;}

		const GiNaC::symbol & GetVariable()const {return variable_ ;}
		bool IsFree()const {return free_;}
		virtual void SetFree(bool free) {free_ = free;}

		bool IsDependent()const {return dependent_;}

		// method for adding this object to the SQLite3 database, needs to be implement by each child class
		virtual void AddToDatabase(sqlite3 *database) = 0;
		virtual void RemoveFromDatabase() {;} // @fixme: change to abstract (=0) 

	protected:
		// if not zero, this is the database where changes to the value of this DOF are stored
		sqlite3 *database_;

		// each instance of this class has a unique ID number
		unsigned id_number_;

		GiNaC::symbol variable_;
		bool free_;

	private:
		bool dependent_;
		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;
};
typedef boost::shared_ptr<DOF> DOFPointer;



#endif //DOFH
