#include "SolverFunctionsBase.h"
#include "Ark3DModel.h"

using namespace std;

SolverFunctionsBase::~SolverFunctionsBase()
{
    dof_list_.clear();
}

void SolverFunctionsBase::AddDOF(DOFPointer new_pointer)
{
    dof_list_.push_back(new_pointer);
}

double SolverFunctionsBase::GetValue(const mmcMatrix &x) const
{
    mmcMatrix local_x = transform_*x;


    for(int i = 0; i < dof_list_.size(); i++)
    {
        if (dof_list_[i]->IsDependent())
        {
            local_x(i,0) = dof_list_[i]->GetSolverFunction()->GetValue(x);
        }
    }

    return GetValueSelf(local_x);
}

mmcMatrix SolverFunctionsBase::GetGradient(const mmcMatrix &x) const
{
    mmcMatrix local_x = transform_*x;
    mmcMatrix jacobian(dof_list_.size(),dof_list_.size(),0.0);

    for(int i = 0; i < dof_list_.size(); i++)
    {
        if (dof_list_[i]->IsDependent())
        {
            local_x(i,0) = dof_list_[i]->GetSolverFunction()->GetValue(x);
            jacobian.SetSubmatrix(0,i,transform_*dof_list_[i]->GetSolverFunction()->GetGradient(x));
        } else {
            jacobian(i,i) = 1.0;
        }
    }

    return transform_.GetTranspose()*jacobian*GetGradientSelf(local_x);
}

void SolverFunctionsBase::DefineInputMap(const std::map<unsigned,unsigned> &input_dof_map)
{
    mmcMatrix new_transform(dof_list_.size(),input_dof_map.size(),0.0);

    map<unsigned,unsigned>::const_iterator map_it;

    for(int i = 0; i < dof_list_.size(); i++)
    {
        if (!dof_list_[i]->IsDependent())
        {
            map_it = input_dof_map.find(dof_list_[i]->GetID());
            if(map_it != input_dof_map.end())
            {
                // dof found in map
                new_transform(i,map_it->second) = 1.0;
            } else {
                // dof not found in map, need to through an exception
                throw Ark3DException("DOF not found in input map while defining transform_ for a SolverFunction.");
            }
        }
    }

    transform_ = new_transform;
}