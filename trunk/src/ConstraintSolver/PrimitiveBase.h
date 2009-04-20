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

#ifndef PrimitiveBaseH
#define PrimitiveBaseH

#include <string>
#include <iostream>
#include <math.h>
#include <vector>
#include "../mmcMatrix/mmcMatrix.h"

#include "DOF.h"

class dimeEntity;

//Exception class
class Ark3DException
{
	public:
		Ark3DException() {std::cerr << "Ark3D exception thrown: No description available." << std::endl;}
		Ark3DException(std::string error_description) {std::cerr << "Ark3D exception thrown: " << error_description << std::endl;}

	private:
	
};

// Selection masks
enum SelectionMask {None, All, Points, Edges, Constraints, Lines, Arcs};


/* Now will define the merit function derived class used in the template matching */
class PrimitiveBase
{
	public:
		PrimitiveBase();
		virtual ~PrimitiveBase() {dof_list_.clear(); primitive_list_.clear();}
		
		// Accessor methods
		void AddDOF(DOFPointer new_dof);  // add a new dof to the DOF list
		void AddPrimitive(boost::shared_ptr<PrimitiveBase> new_primitive);
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}
		const std::vector<boost::shared_ptr<PrimitiveBase> > & GetPrimitiveList() {return primitive_list_;}
		
		unsigned GetID()const {return id_number_;}
		void SetID(int id_number) {id_number_ = id_number;}
		static void SetNextID(int next_id) {next_id_number_ = next_id;}

		void FlagForDeletion() {delete_me_ = true;}
		void UnflagForDeletion() {delete_me_ = false;}
		bool IsFlaggedForDeletion() const {return delete_me_;}
		bool FlagForDeletionIfDependent(boost::shared_ptr<PrimitiveBase> input_primitive);

		bool IsSelectable() const {return selectable_;}

		// selection methods
		virtual bool IsSelected() { return selected_;}
		virtual void SetSelectable(bool selectable);
		virtual void ApplySelectionMask(SelectionMask mask);

		// display methods
		virtual void Display() {;}
		virtual void UpdateDisplay() {;}
		virtual void Erase() {;}

		// Utility method to add the dof_list_ and primitive_list_ to the database
		void DatabaseAddDeleteLists(bool add_to_database, const std::string &dof_list_table_name, const std::string &primitive_list_table_name);

		// method for adding this object to the SQLite3 database, needs to be implement by each child class
		virtual void AddToDatabase(sqlite3 *database) {;} // @fixme: change to a abstract method (=0) so that the compiler finds any child classes that don't implement this method
		virtual void RemoveFromDatabase() {;} // @fixme: change to abstract (=0) 

		// Utility method to sync dof_list_ and primitive_list_ to the database
		void SyncListsToDatabase(const std::string &dof_list_table_name, const std::string &primitive_list_table_name, Ark3DModel &ark3d_model);

		// method to synchronize this object to the database, needs to be implemented by each child class
		// returns true on success, returns false if row does not exist in the database
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model) {;} // @fixme This method should be abstract to insure that all child classes implement it

		virtual dimeEntity *GenerateDimeEntity() const {return 0;}  // used for DXF export

	protected:
		// if not zero, this is the database where changes to the value of this DOF are stored
		sqlite3 *database_;

		bool selected_;
		bool selectable_;

	private:
		std::vector<DOFPointer> dof_list_;
		std::vector< boost::shared_ptr<PrimitiveBase> > primitive_list_;  // list of primitives that this primitive depends on (used for knowing which dependent primitives need to be deleted when deleting a given primitive)

		// each instance of this class has a unique ID number
		unsigned id_number_;

		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;

		// deletion flag used when deleting primitives model
		bool delete_me_; 
};
typedef boost::shared_ptr<PrimitiveBase> PrimitiveBasePointer;


#endif //PrimitiveBaseH
