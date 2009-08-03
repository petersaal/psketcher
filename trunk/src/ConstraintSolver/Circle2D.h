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

#ifndef Circle2DH
#define Circle2DH

#include "Edge2DBase.h"

const std::string SQL_circle2d_database_table_name = "circle2d_list";

const std::string SQL_circle2d_database_schema = "CREATE TABLE " + SQL_circle2d_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, sketch_plane INTEGER NOT NULL, center_point INTEGER NOT NULL, radius_dof INTEGER NOT NULL, s_center_dof INTEGER NOT NULL, t_center_dof INTEGER NOT NULL, text_angle_dof INTEGER NOT NULL, text_radius_dof INTEGER NOT NULL, FOREIGN KEY(id) REFERENCES primitive_list(id), FOREIGN KEY(sketch_plane) REFERENCES primitive_list(id), FOREIGN KEY(center_point) REFERENCES primitive_list(id), FOREIGN KEY(radius_dof) REFERENCES dof_list(id), FOREIGN KEY(s_center_dof) REFERENCES dof_list(id), FOREIGN KEY(t_center_dof) REFERENCES dof_list(id), FOREIGN KEY(text_angle_dof) REFERENCES dof_list(id), FOREIGN KEY(text_radius_dof) REFERENCES dof_list(id));";

// Line2D class
class Circle2D : public Primitive2DBase
{
	public:
		Circle2D (double s_center, double t_center, double radius, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool radius_free = false);
		Circle2D (double s1, double t1, double s2, double t2, double s3, double t3, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool radius_free = false);
		Circle2D (DOFPointer s_center, DOFPointer t_center, DOFPointer radius, SketchPlanePointer sketch_plane);
		Circle2D (unsigned id, pSketcherModel &psketcher_model); // Construct from database

		DOFPointer GetSCenter()const {return s_center_;}
		DOFPointer GetTCenter()const {return t_center_;}

		double GetSCenterValue()const {return s_center_->GetValue();}
		double GetTCenterValue()const {return t_center_->GetValue();}

		DOFPointer GetRadius()const {return radius_;}
		double GetRadiusValue()const {return radius_->GetValue();}
		void SetRadiusValue(double radius, bool update_db=true) {radius_->SetValue(radius,update_db);}

		void Get3DLocations(double & x_center, double & y_center, double & z_center) const;

		Point2DPointer GetCenterPoint() {return center_point_;}

		Point2DPointer GenerateCenterPoint();

		void ApplySelectionMask(SelectionMask mask);

		void SetTextLocation(double text_radius, double text_angle) {text_radius_->SetValue(text_radius); text_angle_->SetValue(text_angle);}
		void SetSTTextLocation(double text_s_, double text_t_, bool update_db=true);
		void SetDefaultTextLocation();
		double GetTextRadius() const {return text_radius_->GetValue();}
		double GetTextAngle() const {return text_angle_->GetValue();}
	
		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

		dimeEntity *GenerateDimeEntity() const; // used for DXF export

	protected:
		// parameters that define the arc
		DOFPointer s_center_;
		DOFPointer t_center_;

		DOFPointer radius_;

		Point2DPointer center_point_;

		DOFPointer text_radius_;
		DOFPointer text_angle_;
};
typedef boost::shared_ptr<Circle2D> Circle2DPointer;



#endif //Circle2DH
