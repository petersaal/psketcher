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

#ifndef HoriVertLine2DH
#define HoriVertLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class HoriVertLine2D : public ConstraintEquationBase
{
	public:
		HoriVertLine2D(const Line2DPointer line, bool vertical_constraint);
		HoriVertLine2D(unsigned id, Ark3DModel &ark3d_model); // Construct from database

		double GetMarkerPosition() const {return marker_position_->GetValue();}
		void SetMarkerPosition(const double marker_position, bool update_db=true) {marker_position_->SetValue(marker_position,update_db);} // @fixme need to maker sure that the new marker position is in the set [0.0,1.0]

		Line2DPointer GetLine() const {return line_;}
		bool IsVerticalConstraint() const {return vertical_constraint_;}
	
		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model);

	protected:
		Line2DPointer line_;

		bool vertical_constraint_;

		DOFPointer marker_position_; // value from 0.0 to 1.0 that determines where the parallel marker will be displayed along each constrained line
};
typedef boost::shared_ptr<HoriVertLine2D> HoriVertLine2DPointer;


#endif //HoriVertLine2DH
