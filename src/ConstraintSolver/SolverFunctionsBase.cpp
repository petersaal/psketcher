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

}

mmcMatrix SolverFunctionsBase::GetGradient(const mmcMatrix &x) const
{

}

void SolverFunctionsBase::DefineInputMap(const std::map<unsigned,unsigned> &input_dof_map)
{
    mmcMatrix new_transform(dof_list_.size(),input_dof_map.size(),0.0);

    map<unsigned,unsigned>::const_iterator map_it;

    for(int i = 0; i < dof_list_.size(); i++)
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

    transform_ = new_transform;
}