/****************************************************************************
**
** This file is part of the pSketcher project.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** Copyright (C) 2006-2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef SolverFunctionsBaseH
#define SolverFunctionsBaseH

#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

#include "DOF.h"
#include "../mmcMatrix/mmcMatrix.h"

//Exception class
class SolverFunctionsException
{
    public:
        SolverFunctionsException() {std::cerr << "SolverFunctions exception thrown: No description available." << std::endl;}
        SolverFunctionsException(std::string error_description) {std::cerr << "SolverFunctions exception thrown: " << error_description << std::endl;}
};

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
        DOFPointer GetDOF(unsigned index) const {return dof_list_[index];}
        unsigned GetNumDOFs() const {return dof_list_.size();}
        const std::vector<DOFPointer> & GetDOFList() const {return dof_list_;}

        // pure abstract methods
        virtual double GetValue() const = 0;
        virtual double GetValueSelf(const mmcMatrix &params) const = 0;
        virtual mmcMatrix GetGradientSelf(const mmcMatrix &params) const = 0;
        virtual std::string GetName() const = 0;

    private:
        std::vector<DOFPointer> dof_list_;
        mmcMatrix transform_;

};
typedef boost::shared_ptr<SolverFunctionsBase> SolverFunctionsBasePointer;

#endif //SolverFunctionsBaseH