#ifndef ParallelLineH
#define ParallelLineH

#include "ConstraintEquationBase.h"
#include "Line.h"

class ParallelLine : public ConstraintEquationBase
{
	public:
		ParallelLine(const LinePointer line1, const LinePointer line2);

	private:

};
typedef boost::shared_ptr<ParallelLine> ParallelLinePointer;


#endif //ParallelLineH
