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

#ifndef DistancePoint2DH
#define DistancePoint2DH

#include "ConstraintEquationBase.h"
#include "Point2D.h"

class DistancePoint2D : public ConstraintEquationBase
{
	public:
		DistancePoint2D(const Point2DPointer point1, const Point2DPointer point2, double distance);
		DistancePoint2D(unsigned id, Ark3DModel &ark3d_model); // Construct from database

		double GetActualDistance();
		double GetValue()const {return distance_->GetValue();}
		void SetValue(double value) {distance_->SetValue(value);}
		void SetTextLocation(double text_position, double text_offset, bool update_db=true) {text_position_->SetValue(text_position,update_db); text_offset_->SetValue(text_offset,update_db);}
		void SetSTTextLocation(double s, double t);
		void SetDefaultTextLocation();
		double GetTextPosition() {return text_position_->GetValue();}	
		double GetTextOffset() {return text_offset_->GetValue();}

		Point2DPointer GetPoint1() const {return point1_;}
		Point2DPointer GetPoint2() const {return point2_;}

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model);

	protected:
		Point2DPointer point1_;
		Point2DPointer point2_;
		DOFPointer distance_;

		DOFPointer text_position_;
		DOFPointer text_offset_;
};
typedef boost::shared_ptr<DistancePoint2D> DistancePoint2DPointer;


#endif //DistancePoint2DH
