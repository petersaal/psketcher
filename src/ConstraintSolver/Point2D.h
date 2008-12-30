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

#ifndef Point2DH
#define Point2DH

#include "Primitive2DBase.h"

// Point2D class (a point constrained to a sketch plane)
class Point2D : public Primitive2DBase
{
	public:
		void ApplySelectionMask(SelectionMask mask);

		Point2D ( double s, double t, SketchPlanePointer sketch_plane, bool s_free = false, bool t_free = false);
		Point2D ( DOFPointer s, DOFPointer t, SketchPlanePointer sketch_plane);

		DOFPointer GetSDOF()const {return s_;}
		DOFPointer GetTDOF()const {return t_;}
		double GetSValue()const {return s_->GetValue();}
		double GetTValue()const {return t_->GetValue();}
		bool SIsFree() const {return s_->IsFree();}
		bool TIsFree() const {return t_->IsFree();}

		void SetSValue(double value){s_->SetValue(value);}
		void SetTValue(double value) {t_->SetValue(value);}
		void SetSFree(bool free) {s_->SetFree(free);}
		void SetTFree(bool free) {t_->SetFree(free);}

		void Get3DLocation(double & x_location, double & y_location, double & z_location)const;

		mmcMatrix GetmmcMatrix()const;

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase

	private:
		DOFPointer s_;
		DOFPointer t_;
};
typedef boost::shared_ptr<Point2D> Point2DPointer;


#endif //Point2DH
