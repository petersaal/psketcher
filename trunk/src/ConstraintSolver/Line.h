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

#ifndef LineH
#define LineH

#include "PrimitiveBase.h"
#include "Point.h"

// line class
class Line : virtual public PrimitiveBase
{
	public:
		Line (const PointPointer point1, const PointPointer point2);

		DOFPointer GetX1()const {return x1_;}
		DOFPointer GetY1()const {return y1_;}
		DOFPointer GetZ1()const {return z1_;}
		DOFPointer GetX2()const {return x2_;}
		DOFPointer GetY2()const {return y2_;}
		DOFPointer GetZ2()const {return z2_;}

		virtual void ApplySelectionMask(SelectionMask mask);

		// method for adding this object to the SQLite3 database
		virtual void AddToDatabase(sqlite3 *database);
		virtual void RemoveFromDatabase();
		void DatabaseAddRemove(bool add_to_database); // Utility method used by AddToDatabase and RemoveFromDatabase

	private:
		DOFPointer x1_;
		DOFPointer y1_;
		DOFPointer z1_;
		DOFPointer x2_;
		DOFPointer y2_;
		DOFPointer z2_;
};
typedef boost::shared_ptr<Line> LinePointer;


#endif //LineH
