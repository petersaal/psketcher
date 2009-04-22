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

#ifndef VectorH
#define VectorH

#include "PrimitiveBase.h"

// vector class
class Vector : virtual public PrimitiveBase
{
	public:
		Vector ( double x, double y, double z, bool x_free = false, bool y_free = false, bool z_free = false);
		//Vector ( DOFPointer x, DOFPointer y, DOFPointer z );
		Vector (unsigned id, Ark3DModel &ark3d_model); // Construct from database

		double GetXValue()const {return x_->GetValue();}
		double GetYValue()const {return y_->GetValue();}
		double GetZValue()const {return z_->GetValue();}

		mmcMatrix GetmmcMatrix();  // returns mmcMatrix vector containing current location 

		void ApplySelectionMask(SelectionMask mask);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase
		virtual bool SyncToDatabase(Ark3DModel &ark3d_model);

	private:
		DOFPointer x_;
		DOFPointer y_;
		DOFPointer z_;
};
typedef boost::shared_ptr<Vector> VectorPointer;



#endif //VectorH
