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
		void SetValue ( double value ) {value_ = value;}
		double GetValue()const {return value_;}

	private:
		double value_;
};
typedef boost::shared_ptr<IndependentDOF> IndependentDOFPointer;


#endif //IndependentDOFH
