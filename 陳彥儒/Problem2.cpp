#include "basicDS.h"
#include "Problem2.h"

using namespace std;
using EdgePQ = priority_queue<graphEdge*, vector<graphEdge*>, CompareEdge>;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */


Problem2::Problem2(Graph G) {
	/* Write your code here. */
	
}

Problem2::~Problem2() {
	/* Write your code here. */

}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */

	/* You should return true or false according the insertion result */
	return true;
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */
	
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	
	return;
}

int CompareEdge::computePoint(graphEdge *edge)
{ 
	auto degreeMap = Problem2::vertiexDegreeMap;
	return square(degreeMap[edge->vertex[0]]) + square(degreeMap[edge->vertex[0]]) - edge->ce;
}