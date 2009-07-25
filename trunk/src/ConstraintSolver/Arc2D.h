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

#ifndef Arc2DH
#define Arc2DH

#include "Edge2DBase.h"

// Line2D class
class Arc2D : public Edge2DBase
{
	public:
		Arc2D (double s_center, double t_center, double theta_1, double theta_2, double radius, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, bool radius_free = false);
		Arc2D (double s1, double t1, double s2, double t2, double s3, double t3, SketchPlanePointer sketch_plane,
               bool s_center_free = false, bool t_center_free = false, bool theta_1_free = false, bool theta_2_free = false, bool radius_free = false);
		Arc2D (DOFPointer s_center, DOFPointer t_center, DOFPointer theta_1, DOFPointer theta_2, DOFPointer radius, SketchPlanePointer sketch_plane);
		Arc2D (unsigned id, pSketcherModel &psketcher_model); // Construct from database

		DOFPointer GetSCenter()const {return s_center_;}
		DOFPointer GetTCenter()const {return t_center_;}

		double GetSCenterValue()const {return s_center_->GetValue();}
		double GetTCenterValue()const {return t_center_->GetValue();}

		DOFPointer GetTheta1()const {return theta_1_;}
		DOFPointer GetTheta2()const {return theta_2_;}

		double GetTheta1Value()const {return theta_1_->GetValue();}
		double GetTheta2Value()const {return theta_2_->GetValue();}

		DOFPointer GetRadius()const {return radius_;}
		double GetRadiusValue()const {return radius_->GetValue();}
		void SetRadiusValue(double radius) {radius_->SetValue(radius);}

		void Get3DLocations(double & x_center, double & y_center, double & z_center) const;

		Point2DPointer GetPoint1(){return point1_;}
		Point2DPointer GetPoint2(){return point2_;}
		Point2DPointer GetCenterPoint() {return center_point_;}

		Point2DPointer GeneratePoint1();
		Point2DPointer GeneratePoint2();
		Point2DPointer GenerateCenterPoint();

		void GetTangent1(DOFPointer & s_component, DOFPointer & t_component);  // returns a dependent that defines tangent vector for each endpoint of the edge
		void GetTangent2(DOFPointer & s_component, DOFPointer & t_component);

		void GetTangent1(double & s_component, double & t_component);
		void GetTangent2(double & s_component, double & t_component);

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

		DOFPointer theta_1_;	// start angle
		DOFPointer theta_2_;	// end angle

		DOFPointer radius_;

		// Points based on dependent DOF's that provide the location of the arc endpoints
		// It is important that these points be maintained here so that Edge2DCBase point coincident checks can be performed by comparing DOF pointers
		// If these points are generated for each call to GetPoint1() then, for every call to GetPoint*(), the pointers to the S and T DOF's will be unique
		Point2DPointer point1_;
		Point2DPointer point2_;
		Point2DPointer center_point_;

		DOFPointer text_radius_;
		DOFPointer text_angle_;
};
typedef boost::shared_ptr<Arc2D> Arc2DPointer;



#endif //Arc2DH
