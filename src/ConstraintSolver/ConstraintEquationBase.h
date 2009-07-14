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
** Copyright (C) 2006-2008 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef ConstraintEquationBaseH
#define ConstraintEquationBaseH

#include "PrimitiveBase.h"
#include "SolverFunctions.h"

// constraint equation class
class ConstraintEquationBase : virtual public PrimitiveBase
{
	public:
		virtual ~ConstraintEquationBase() {constraints_.clear();}
	
		// Accessor methods
		const std::vector< boost::shared_ptr<GiNaC::ex> > & GetConstraintList() {return constraints_;}
		const std::vector<double> & GetWeightList() {return weight_list_;}

		virtual void ApplySelectionMask(SelectionMask mask);

		// Utility method to add the constraints_ list to the database
		void DatabaseAddDeleteConstraintList(bool add_to_database, const std::string &constraint_list_table_name);

		// utility method to synchronize the contraints_ and weight_list_ vectors to the database
		// Important: this method assumes that the method PrimitiveBase::SyncListsToDatabase(...) has been called prior to calling this method (needs the dof_list_ vector up to date)
		void SyncConstraintListToDatabase(const std::string &constraint_list_table_name, Ark3DModel &ark3d_model);

	protected:
		// constraints and weight_list_ are parallel vectors
		// stores constraints
		std::vector< boost::shared_ptr<GiNaC::ex> > constraints_; 

		// this vector stores the weights for the constraints
		std::vector< double > weight_list_;
};
typedef boost::shared_ptr<ConstraintEquationBase> ConstraintEquationBasePointer;


#endif //ConstraintEquationBaseH
