#include "AngleLine2D.h"

#include "IndependentDOF.h"

using namespace std;
using namespace GiNaC;

// Create an angle constraint between two lines
AngleLine2D::AngleLine2D(const Line2DPointer line1, const Line2DPointer line2, double angle /* radians */, bool interior_angle):
line1_(line1),
line2_(line2),
interior_angle_(interior_angle)
{
	SetDefaultTextLocation();

	// Create a DOF for the angle parameter
	DOFPointer new_dof(new IndependentDOF(angle,false));
	angle_ = new_dof;	

	dof_list_.push_back(angle_);

	// create the expression that defines the parallel constraint and add it the the constraint list
	boost::shared_ptr<ex> new_constraint(new ex);
	
	ex line1_ds = line1->GetS2()->GetVariable() - line1->GetS1()->GetVariable();
	ex line1_dt = line1->GetT2()->GetVariable() - line1->GetT1()->GetVariable();
	ex line1_length = sqrt(pow(line1_ds,2)+pow(line1_dt,2));

	ex line2_ds = line2->GetS2()->GetVariable() - line2->GetS1()->GetVariable();
	ex line2_dt = line2->GetT2()->GetVariable() - line2->GetT1()->GetVariable();
	ex line2_length = sqrt(pow(line2_ds,2)+pow(line2_dt,2));

	ex desired_angle = angle_->GetVariable();

	// Calculate the dot product normalized by the vector lengths and subtract the cos of the desired angle
	// this expression will be zero when the lines are at the desired angle
	if(interior_angle_)
		*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(desired_angle);
	else
		*new_constraint = (1/(line1_length*line2_length))*(line1_ds*line2_ds + line1_dt*line2_dt)-cos(mmcPI-desired_angle);

	constraints_.push_back(new_constraint);
	weight_list_.push_back(1.0);
}

void AngleLine2D::SetDefaultTextLocation()
{
	// first determine the intersection point of the two lines
	double x1 = line1_->GetPoint1()->GetSValue();
	double x2 = line1_->GetPoint2()->GetSValue();
	double x3 = line2_->GetPoint1()->GetSValue();
	double x4 = line2_->GetPoint2()->GetSValue();

	double y1 = line1_->GetPoint1()->GetTValue();
	double y2 = line1_->GetPoint2()->GetTValue();
	double y3 = line2_->GetPoint1()->GetTValue();
	double y4 = line2_->GetPoint2()->GetTValue();

	double denominator = (x1-x2)*(y3-y4)-(x3-x4)*(y1-y2);

	if(denominator == 0.0)
	{
		// Lines are parallel so there is no sense in defining text_radius_ and text_angle_ since there is no intersection point to reference it to
		text_radius_ = 0.0;
		text_angle_ = 0.0;
	} else {
		// lines do intersect
		// finish calculating the intersection point
		double temp1 = x1*y2-y1*x2;
		double temp2 = x3*y4-x4*y3;
		
		double x_center = (temp1*(x3-x4)-temp2*(x1-x2))/denominator;
		double y_center = (temp1*(y3-y4)-temp2*(y1-y2))/denominator;
		
		// calculate the average radius and the average angle of all of the line endpoints
		double ave_radius = 0.0;
		double ave_angle = 0.0;
		
		ave_radius += sqrt((x1-x_center)*(x1-x_center)+(y1-y_center)*(y1-y_center));
		ave_radius += sqrt((x2-x_center)*(x2-x_center)+(y2-y_center)*(y2-y_center));
		ave_radius += sqrt((x3-x_center)*(x3-x_center)+(y3-y_center)*(y3-y_center));
		ave_radius += sqrt((x4-x_center)*(x4-x_center)+(y4-y_center)*(y4-y_center));
		ave_radius = ave_radius/4.0;

		int angle_counter = 0;
		if((y1-y_center)*(y1-y_center)+(x1-x_center)*(x1-x_center) > 0)
		{
			ave_angle += atan2(y1-y_center,x1-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y2-y_center)*(y2-y_center)+(x2-x_center)*(x2-x_center) > 0)
		{
			ave_angle += atan2(y2-y_center,x2-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y3-y_center)*(y3-y_center)+(x3-x_center)*(x3-x_center) > 0)
		{
			ave_angle += atan2(y3-y_center,x3-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if((y4-y_center)*(y4-y_center)+(x4-x_center)*(x4-x_center) > 0)
		{
			ave_angle += atan2(y4-y_center,x4-x_center) + 2.0*mmcPI;
			angle_counter++;
		}

		if(angle_counter > 0)
			ave_angle = ave_angle / (double)angle_counter;
		else
			ave_angle = 0.0;

		text_radius_ = ave_radius*0.75;
		text_angle_ = ave_angle;
	}
}
