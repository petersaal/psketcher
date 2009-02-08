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

#ifndef DOFH
#define DOFH

#include <boost/shared_ptr.hpp>
#include <ginac/ginac.h>
#include <sqlite3.h>

class Ark3DModel;

// Abstract DOF base class
class DOF
{
	public:
		DOF (bool free, bool dependent);
		DOF (const char *name, bool free, bool dependent);
		DOF (unsigned id, bool dependent); // used when creating a DOF from the sqlite database

		virtual ~DOF() {;}

		//Accessor methods
		virtual void SetValue ( double value ) = 0;
		virtual double GetValue()const = 0;
		virtual GiNaC::ex GetExpression()const {return variable_;}

		unsigned GetID()const {return id_number_;}
		void SetID(int id_number) {id_number_ = id_number; next_id_number_ = id_number_ >= next_id_number_ ? id_number_ : next_id_number_;} 

		const GiNaC::symbol & GetVariable()const {return variable_ ;}
		bool IsFree()const {return free_;}
		virtual void SetFree(bool free) {free_ = free;}

		bool IsDependent()const {return dependent_;}

		void FlagForDeletion() {delete_me_ = true;}
		bool IsFlaggedForDeletion() const {return delete_me_;}

		// method for adding this object to the SQLite3 database, needs to be implement by each child class
		virtual void AddToDatabase(sqlite3 *database) = 0;
		virtual void RemoveFromDatabase() = 0;

		// method to synchronize this object to the database, needs to be implemented by each child class
		// returns true on success, returns false if row does not exist in the database
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model) = 0;

	protected:
		// if not zero, this is the database where changes to the value of this DOF are stored
		sqlite3 *database_;

		GiNaC::symbol variable_;
		bool free_;

	private:
		// deletion flag used when deleting primitives model
		bool delete_me_;

		bool dependent_;
		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;

		// each instance of this class has a unique ID number
		unsigned id_number_;
};
typedef boost::shared_ptr<DOF> DOFPointer;



#endif //DOFH
