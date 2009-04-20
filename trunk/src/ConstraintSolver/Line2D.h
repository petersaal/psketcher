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

#ifndef Line2DH
#define Line2DH

#include "Edge2DBase.h"
#include "Point2D.h"

// Line2D class
class Line2D : public Edge2DBase
{
	public:
		Line2D (const Point2DPointer point1, const Point2DPointer point2, SketchPlanePointer sketch_plane);
		Line2D (unsigned id, Ark3DModel &ark3d_model); // Construct from database

		DOFPointer GetS1()const {return s1_;}
		DOFPointer GetT1()const {return t1_;}

		DOFPointer GetS2()const {return s2_;}
		DOFPointer GetT2()const {return t2_;}

		void Get3DLocations(double & x1, double & y1, double & z1,
												double & x2, double & y2, double & z2)const ;

		Point2DPointer GetPoint1();
		Point2DPointer GetPoint2();

		void GetTangent1(GiNaC::ex & s_component, GiNaC::ex & t_component, std::vector<DOFPointer> & dof_list);  // returns expression that defines tangent vector for each endpoint of the edge and appends any DOF's that the expression depends on onto the vector dof_list
		void GetTangent2(GiNaC::ex & s_component, GiNaC::ex & t_component, std::vector<DOFPointer> & dof_list);

		void GetTangent1(double & s_component, double & t_component);
		void GetTangent2(double & s_component, double & t_component);

		void ApplySelectionMask(SelectionMask mask);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model);

		virtual dimeEntity *GenerateDimeEntity() const;

	private:
		DOFPointer s1_;
		DOFPointer t1_;

		DOFPointer s2_;
		DOFPointer t2_;

		// point1 and point2 do not need to be put into the database since they are created on the fly as needed
		Point2DPointer point1_;  // point1 and point2 depend on s1 through t2 and are included just so they don't have to be generated for every call to GetPoint*()
		Point2DPointer point2_;
};
typedef boost::shared_ptr<Line2D> Line2DPointer;


#endif //Line2DH
