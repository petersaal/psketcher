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

#ifndef AngleLine2DH
#define AngleLine2DH

#include "ConstraintEquationBase.h"
#include "Line2D.h"

class AngleLine2D : public ConstraintEquationBase
{
	public:
		AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */, bool interior_angle);
		AngleLine2D(unsigned id, pSketcherModel &psketcher_model); // Construct from database
	
		void SetTextLocation(double text_radius, double text_angle) {text_radius_->SetValue(text_radius); text_angle_->SetValue(text_angle);}
		void SetSTTextLocation(double text_s, double text_t, bool update_db=true);
		void SetDefaultTextLocation();
		double GetTextRadius() const {return text_radius_->GetValue();}	
		double GetTextAngle() const {return text_angle_->GetValue();}

		Line2DPointer GetLine1() const {return line1_;}
		Line2DPointer GetLine2() const {return line2_;}

		bool IsInteriorAngle() const {return interior_angle_;}

		double GetAngleValue() const {return angle_->GetValue();}
		void SetAngleValue(double angle) {angle_->SetValue(angle);}

		double GetActualAngle() const;

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);
		
	protected:
		Line2DPointer line1_;
		Line2DPointer line2_;

		DOFPointer angle_;
		
		bool interior_angle_;

		DOFPointer text_radius_;
		DOFPointer text_angle_;

		// the following two parameters are used to position the angle dimension label if line1_ and line2_ happen to be exactly parallel
		DOFPointer text_s_;
		DOFPointer text_t_;
};
typedef boost::shared_ptr<AngleLine2D> AngleLine2DPointer;



#endif //AngleLine2DH
