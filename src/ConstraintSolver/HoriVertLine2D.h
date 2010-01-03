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

const std::string SQL_horivert_line2d_database_table_name = "horivert_line2d_list";

const std::string SQL_horivert_line2d_database_schema = "CREATE TABLE " + SQL_horivert_line2d_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, line INTEGER NOT NULL, bool_vertical_constraint INTEGER INTEGER CHECK (bool_vertical_constraint >= 0 AND bool_vertical_constraint <= 1), marker_position_dof INTEGER NOT NULL, weight FLOAT NOT NULL, FOREIGN KEY(id) REFERENCES constraint_equation_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(line) REFERENCES primitive_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(marker_position_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED);";

class HoriVertLine2D : public ConstraintEquationBase
{
	public:
		HoriVertLine2D(const Line2DPointer line, bool vertical_constraint);
		HoriVertLine2D(unsigned id, pSketcherModel &psketcher_model); // Construct from database

		double GetMarkerPosition() const {return marker_position_->GetValue();}
		void SetMarkerPosition(const double marker_position, bool update_db=true) {marker_position_->SetValue(marker_position,update_db);} // @fixme need to maker sure that the new marker position is in the set [0.0,1.0]

		Line2DPointer GetLine() const {return line_;}
		bool IsVerticalConstraint() const {return vertical_constraint_;}
	
		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	protected:
		Line2DPointer line_;

		bool vertical_constraint_;

		DOFPointer marker_position_; // value from 0.0 to 1.0 that determines where the parallel marker will be displayed along each constrained line
};
typedef boost::shared_ptr<HoriVertLine2D> HoriVertLine2DPointer;


#endif //HoriVertLine2DH
