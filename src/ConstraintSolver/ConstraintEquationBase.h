#ifndef ConstraintEquationBaseH
#define ConstraintEquationBaseH

#include "PrimitiveBase.h"

// constraint equation class
class ConstraintEquationBase : public PrimitiveBase
{
	public:
		virtual ~ConstraintEquationBase() {constraints_.clear();}
	
		// Accessor methods
		const std::vector< boost::shared_ptr<GiNaC::ex> > & GetConstraintList() {return constraints_;}
		const std::vector<double> & GetWeightList() {return weight_list_;}

		virtual void ApplySelectionMask(SelectionMask mask);

		// Utility method to add the constraints_ list to the database
		void AddConstraintListToDatabase(const std::string &constraint_list_table_name);

	protected:
		// constraints and weight_list_ are parallel vectors
		// stores constraints
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_; 

		// this vector stores the weights for the constraints
		std::vector< double > weight_list_;
};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;


#endif //ConstraintEquationBaseH
