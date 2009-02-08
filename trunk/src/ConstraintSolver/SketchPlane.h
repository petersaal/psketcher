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

#ifndef SketchPlaneH
#define SketchPlaneH

#include "PrimitiveBase.h"
#include "Vector.h"
#include "Point.h"

// sketch plane class (includes up vector)
class SketchPlane : public PrimitiveBase
{
	public:
		SketchPlane ( VectorPointer normal, VectorPointer up, PointPointer base);
		SketchPlane (unsigned id, Ark3DModel &ark3d_model); // Construct from database

		void Get3DLocation ( double s, double t, double & x, double & y, double & z);
		double GetSTLocation( double x, double y, double z, double &s, double &t);
		void GetABCD ( double & coef_a, double & coef_b, double & coef_c, double & coef_d);
		
		void ApplySelectionMask(SelectionMask mask);

		VectorPointer GetNormal() {return normal_;}
		VectorPointer GetUp() {return up_;}
		PointPointer GetBase() {return base_;}

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(unsigned id, Ark3DModel &ark3d_model);

	private:
		VectorPointer normal_;
		VectorPointer up_;
		PointPointer base_;
};
typedef boost::shared_ptr<SketchPlane> SketchPlanePointer;


#endif //SketchPlaneH
