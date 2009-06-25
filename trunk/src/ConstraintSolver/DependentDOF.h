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
		// the following constructor creates the DOF from the database stored in ark3d_model
		DependentDOF ( unsigned id, Ark3DModel &ark3d_model );

		~DependentDOF () {source_dof_list_.clear();}
		
		//Accessor methods
		void SetValue ( double value, bool update_db = true ) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		void SetFree(bool free) { /* @fixme warn user about an attempt to modify a dependent DOF */;}
		double GetValue()const;
		GiNaC::ex GetExpression()const;
		const std::vector<DOFPointer> & GetDOFList() {return source_dof_list_;}

		// methods for adding and removing this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddDelete(bool add_to_database); // utility method called by AddToDatabase and DeleteFromDatabase since they both do similar things

		// method to synchronize this object to the database
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model);

	private:
		GiNaC::ex expression_;
		std::vector<DOFPointer> source_dof_list_;
};
typedef boost::shared_ptr<DependentDOF> DependentDOFPointer;



#endif //DependentDOFH
