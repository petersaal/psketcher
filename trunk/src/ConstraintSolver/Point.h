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

#ifndef PointH
#define PointH

#include "PrimitiveBase.h"

const std::string SQL_point_database_table_name = "point_list";

const std::string SQL_point_database_schema = "CREATE TABLE " + SQL_point_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, x_dof INTEGER NOT NULL, y_dof INTEGER NOT NULL, z_dof INTEGER NOT NULL, FOREIGN KEY(id) REFERENCES primitive_list(id), FOREIGN KEY(x_dof) REFERENCES dof_list(id), FOREIGN KEY(y_dof) REFERENCES dof_list(id), FOREIGN KEY(z_dof) REFERENCES dof_list(id));";

// point class
class Point : virtual public PrimitiveBase
{
	public:
		Point ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Point ( DOFPointer x, DOFPointer y, DOFPointer z );
		Point (unsigned id, pSketcherModel &psketcher_model); // Construct from database

		void ApplySelectionMask(SelectionMask mask);

		DOFPointer GetXDOF()const {return x_;}
		DOFPointer GetYDOF()const {return y_;}
		DOFPointer GetZDOF()const {return z_;}

		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Point> PointPointer;


#endif //PointH
