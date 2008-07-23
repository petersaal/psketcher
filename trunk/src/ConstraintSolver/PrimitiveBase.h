#ifndef PrimitiveBaseH
#define PrimitiveBaseH

#include <math.h>
#include <vector>
#include "../mmcMatrix/mmcMatrix.h"

#include "DOF.h"



// Selection masks
enum SelectionMask {None, All, Points, Edges, Constraints, Lines, Arcs};


/* Now will define the merit function derived class used in the template matching */
class PrimitiveBase
{
	public:
		PrimitiveBase();
		virtual ~PrimitiveBase() {dof_list_.clear();}
		
		// Accessor methods
		void AddDOF(DOFPointer new_dof);  // add a new dof to the DOF list
		const std::vector<DOFPointer> & GetDOFList() {return dof_list_;}	
		unsigned GetID()const {return id_number_;}

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

		// each instance of this class has a unique ID number
		unsigned id_number_;

		// static variable used to provide a unique ID number to each instance of this class
		static unsigned next_id_number_;
};
typedef boost::shared_ptr<PrimitiveBase> PrimitiveBasePointer;


#endif //PrimitiveBaseH
