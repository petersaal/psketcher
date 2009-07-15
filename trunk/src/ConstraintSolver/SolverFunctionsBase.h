#ifndef SolverFunctionsBaseH
#define SolverFunctionsBaseH

#include <boost/shared_ptr.hpp>
#include <vector>

#include "DOF.h"
#include "../mmcMatrix/mmcMatrix.h"

// Abstract Solver Function base class
class SolverFunctionsBase
{
    public:
        virtual ~SolverFunctionsBase();

        // methods implemented by the base class
        void AddDOF(DOFPointer new_pointer);
        double GetValue(const mmcMatrix &x) const;
        mmcMatrix GetGradient(const mmcMatrix &x) const;
        void DefineInputMap(const std::map<unsigned,unsigned> &input_dof_map);

        // pure abstract methods
        virtual double GetValueSelf(const mmcMatrix &params) const = 0;
        virtual mmcMatrix GetGradientSelf(const mmcMatrix &params) const = 0;

    private:
        std::vector<DOFPointer> dof_list_;
        mmcMatrix transform_;

};
typedef boost::shared_ptr<SolverFunctionsBase> SolverFunctionsBasePointer;




#endif //SolverFunctionsBaseH