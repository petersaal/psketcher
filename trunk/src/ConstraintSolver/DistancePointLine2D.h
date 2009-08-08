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
** Copyright (C) 2009 Michael Greminger. All rights reserved.
**
****************************************************************************/

#ifndef DistancePointLine2DH
#define DistancePointLine2DH

#include "ConstraintEquationBase.h"
#include "Point2D.h"
#include "Line2D.h"

const std::string SQL_distance_pointline2d_database_table_name = "distance_pointline2d_list";

const std::string SQL_distance_pointline2d_database_schema = "CREATE TABLE " + SQL_distance_pointline2d_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, distance_dof INTEGER NOT NULL, point INTEGER NOT NULL, line INTEGER NOT NULL, text_offset_dof INTEGER NOT NULL, text_position_dof INTEGER NOT NULL, weight FLOAT NOT NULL, FOREIGN KEY(id) REFERENCES constraint_equation_list(id), FOREIGN KEY(distance_dof) REFERENCES dof_list(id), FOREIGN KEY(point) REFERENCES primitive_list(id), FOREIGN KEY(line) REFERENCES primitive_list(id), FOREIGN KEY(text_offset_dof) REFERENCES dof_list(id), FOREIGN KEY(text_position_dof) REFERENCES dof_list(id));";

class DistancePointLine2D : public ConstraintEquationBase
{
	public:
		DistancePointLine2D(const Point2DPointer point, const Line2DPointer line, double distance);
		DistancePointLine2D(unsigned id, pSketcherModel &psketcher_model); // Construct from database

		double GetActualDistance() const;
		double GetValue()const {return distance_->GetValue();}
		void SetValue(double value) {distance_->SetValue(value);}
		void SetTextLocation(double text_position, double text_offset, bool update_db=true) {text_position_->SetValue(text_position,update_db); text_offset_->SetValue(text_offset,update_db);}
		void SetSTTextLocation(double s, double t, bool update_db=true);
		void SetDefaultTextLocation();
		double GetTextPosition() {return text_position_->GetValue();}
		double GetTextOffset() {return text_offset_->GetValue();}

		Point2DPointer GetPoint() const {return point_;}
		Line2DPointer GetLine() const {return line_;}

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	protected:
		Point2DPointer point_;
		Line2DPointer line_;
		DOFPointer distance_;

		DOFPointer text_position_;
		DOFPointer text_offset_;
};
typedef boost::shared_ptr<DistancePointLine2D> DistancePointLine2DPointer;


#endif //DistancePointLine2DH
