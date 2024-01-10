#include "basicDS.h"
#include "Problem1.h"

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */


Problem1::Problem1(Graph G) {
	/* Write your code here. */
	numOfV = G.V.size(); for(int i=0; i<G.E.size(); i++) 
	edgesMap[G.E[i].vertex[0]] = &G.E[i];
}

Problem1::~Problem1() {
	/* Write your code here. */

}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	/* Write your code here. */
	VertexDisjointSet vertex_dset(numOfV);
	vector<graphEdge&> MTEdges_G;
	priority_queue<graphEdge*, vector<graphEdge*>, CompareEdge> edge_pq;
	for(int i=0; i<G.E.size(); i++) if(G.E[i].b >= t) edge_pq.push(&G.E[i]);
	// spanning tree
	while (!edge_pq.empty() && vertex_dset.numOfRoot() > 1)
	{
		graphEdge *newEdge = edge_pq.top();
		edge_pq.pop();
		if(vertex_dset.find(newEdge->vertex[0]) != vertex_dset.find(newEdge->vertex[1]))
		{
			MTEdges_G.push_back(*newEdge);
			vertex_dset.unionSets(newEdge->vertex[0], newEdge->vertex[1]);
		}
	}
	// deal with connected edge and the effect of the edge
	int ct = 0, rootOfSourse = vertex_dset.find(s);
	vector<treeEdge> MTEdges;
	for(auto edge: MTEdges_G)
	{
		if(vertex_dset.find(edge->vertex[0]) == rootOfSourse)
		{
			MTEdges.push_back( { edge->vertex[0], edge->vertex[1]} );
			edge->b -= t;
			ct += edge->ce * t;
		}
	}
	// find connected vertax
	vector<int> MTVertaces;
	for(int vertex = 1; vertex<=numOfV; vertex++) 
		if(vertex_dset.find(vertex) == rootOfSourse)
			MTVertaces.push_back(vertex);
	// output tree
	MTid = { MTVertaces, MTEdges, s, id, ct};
	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	/* Write your code here. */
	for(auto edge: MTidForest.trees[id].E)
	{

	}

	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	
	return;
}

// Tree& findTree(int id, Forest &forest) 
// { 
// 	function<int(int, int)> findTreeIterator = [&] (int l, int r) -> int
// 	{
// 		if(forest.trees[(l+r) / 2].id == id) return (l+r) / 2;
// 		if(forest.trees[(l+r) / 2].id > id) return findTreeIterator(l, (l+r) / 2);
// 		return findTreeIterator((l+r) / 2 + 1, r);
// 	};
// 	return forest.trees[findTreeIterator(0, forest.size)]; 
// }

VertexDisjointSet::VertexDisjointSet(int numberOfV) {
	d_set = new pair<int, int>[numberOfV + 1];
	numRoots = 0;
    for (int vertex = 1; vertex<=numberOfV; vertex++) {
        d_set[vertex] = std::make_pair(-1, 0);
		numRoots++;
    }
}

VertexDisjointSet::~VertexDisjointSet() { delete[] d_set; }

int VertexDisjointSet::find(int x) {
    if (d_set[x].first < 0)
        return x;
    else
        return d_set[x].first = find(d_set[x].first);
}

void VertexDisjointSet::unionSets(int v1, int v2) {
    int root1 = find(v1);
    int root2 = find(v2);

    if (root1 == root2)
        return;

    // Union by rank
    if (d_set[root1].second < d_set[root2].second) {
        d_set[root1].first = root2;
    } else {
        d_set[root2].first = root1;
    } 
	if (d_set[root1].second == d_set[root2].second) d_set[root1].second++;
    numRoots--;
}
