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
