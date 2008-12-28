#ifndef DependentDOFH
#define DependentDOFH

#include <vector>

#include "DOF.h"

// DependentDOF class
class DependentDOF : public DOF
{
	public:
		DependentDOF ( GiNaC::ex expression, std::vector<DOFPointer> source_dof_list);
		DependentDOF ( const char *name, GiNaC::ex expression, std::vector<DOFPointer> source_dof_list);

		~DependentDOF () {source_dof_list_.clear();}
		
		//Accessor methods
		void SetValue ( double value ) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		void SetFree(bool free) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		double GetValue()const;
		GiNaC::ex GetExpression()const;
		const std::vector<DOFPointer> & GetDOFList() {return source_dof_list_;}

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database)const;

	private:
		GiNaC::ex expression_;
		std::vector<DOFPointer> source_dof_list_;
};
typedef boost::shared_ptr<DependentDOF> DependentDOFPointer;



#endif //DependentDOFH
