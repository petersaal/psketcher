#ifndef PrimitiveBaseH
#define PrimitiveBaseH

#include <math.h>
#include <vector>
#include "../mmcMatrix/mmcMatrix.h"

#include "DOF.h"

//Exception class
class Ark3DException{};

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
		void AddPrimitive(boost::shared_ptr<PrimitiveBase> new_primitive) {primitive_list_.push_back(new_primitive);}
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}	
		unsigned GetID()const {return id_number_;}

		void FlagForDeletion() {delete_me_ = true;}
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


	protected:
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
