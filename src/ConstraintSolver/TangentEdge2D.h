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

#ifndef TangentEdge2DH
#define TangentEdge2DH

#include "ConstraintEquationBase.h"
#include "Edge2DBase.h"

const std::string SQL_tangent_edge2d_database_table_name = "tangent_edge2d_list";

const std::string SQL_tangent_edge2d_database_schema = "CREATE TABLE " + SQL_tangent_edge2d_database_table_name + " (id INTEGER PRIMARY KEY, dof_table_name TEXT NOT NULL, primitive_table_name TEXT NOT NULL, edge1 INTEGER NOT NULL, edge2 INTEGER NOT NULL, point_num_1 INTEGER NOT NULL, point_num_2 INTEGER NOT NULL, s_1_dof INTEGER NOT NULL, t_1_dof INTEGER NOT NULL, s_2_dof INTEGER NOT NULL, t_2_dof INTEGER NOT NULL, weight FLOAT NOT NULL, FOREIGN KEY(id) REFERENCES constraint_equation_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(edge1) REFERENCES primitive_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(edge2) REFERENCES primitive_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(s_1_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(t_1_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(s_2_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED, FOREIGN KEY(t_2_dof) REFERENCES dof_list(id) ON UPDATE CASCADE DEFERRABLE INITIALLY DEFERRED);";

class TangentEdge2D : public ConstraintEquationBase
{
	public:
		TangentEdge2D(Edge2DBasePointer edge1, EdgePointNumber point_num_1, Edge2DBasePointer edge2, EdgePointNumber point_num_2);
		TangentEdge2D(unsigned id, pSketcherModel &psketcher_model); // Construct from database

		// Accessor methods
		Edge2DBasePointer GetEdge1() const {return edge1_;}
		Edge2DBasePointer GetEdge2() const {return edge2_;}

		EdgePointNumber GetPointNum1() const {return point_num_1_;}
		EdgePointNumber GetPointNum2() const {return point_num_2_;}

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(pSketcherModel &psketcher_model);

	protected:
		Edge2DBasePointer edge1_;
		Edge2DBasePointer edge2_;

		EdgePointNumber point_num_1_;
		EdgePointNumber point_num_2_;

        DOFPointer s_1_;
        DOFPointer t_1_;
        DOFPointer s_2_;
        DOFPointer t_2_;
};
typedef boost::shared_ptr<TangentEdge2D> TangentEdge2DPointer;

#endif //TangentEdge2DH
