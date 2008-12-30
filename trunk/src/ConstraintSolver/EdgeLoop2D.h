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

#ifndef EdgeLoop2DH
#define EdgeLoop2DH


#include "Edge2DBase.h"

// 2D edge loop classs
class EdgeLoop2D
{
	public:
		// constructor and destructor
		EdgeLoop2D() {;}
		EdgeLoop2D(std::vector<Edge2DBasePointer> edge_list);
		~EdgeLoop2D() {edge_list_.clear();}

		// Acessor methods
		void AddEdge(Edge2DBasePointer new_edge);
		
		// utility methods
		bool IsLoopValid();		// Check to insure that the loop is well defined

	private:
		std::vector<Edge2DBasePointer> edge_list_;
};
typedef boost::shared_ptr<EdgeLoop2D> EdgeLoop2DPointer;


#endif //EdgeLoop2DH
