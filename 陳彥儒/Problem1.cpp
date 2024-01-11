#include "basicDS.h"
#include "Problem1.h"

using namespace std;
using EdgePQ = priority_queue<graphEdge*, vector<graphEdge*>, CompareEdge>;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

Problem1::Problem1(Graph G) {
	/* Write your code here. */
	numOfV = G.V.size(); 
	for(int i=0; i<G.E.size(); i++) 
		edgesMap[G.E[i].vertex[0]][G.E[i].vertex[1]] = &G.E[i];
}

Problem1::~Problem1() {
	/* Write your code here. */

}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	/* Write your code here. */
	// init
	VertexDisjointSet vertex_dset(numOfV);
	vector<graphEdge*> MTEdges_G;
	EdgePQ edge_pq;
	for(int i=0; i<G.E.size(); i++) if(G.E[i].b >= t) edge_pq.push(&G.E[i]);
	// spanning tree
	while (!edge_pq.empty() && vertex_dset.numOfRoot() > 1)
	{
		graphEdge *newEdge = edge_pq.top();
		edge_pq.pop();
		if(vertex_dset.find(newEdge->vertex[0]) != vertex_dset.find(newEdge->vertex[1]))
		{
			MTEdges_G.push_back(newEdge);
			vertex_dset.unionSets(newEdge->vertex[0], newEdge->vertex[1]);
		}
	}
	// deal with effect of traffic
	int ct = 0, rootOfSourse = vertex_dset.find(s);
	vector<treeEdge> MTEdges;
	for(auto edge: MTEdges_G)
	{
		if(vertex_dset.find(edge->vertex[0]) == rootOfSourse) // exclude unconnected edges
		{
			MTEdges.push_back( { edge->vertex[0], edge->vertex[1]} );
			edge->b -= t;
			ct += edge->ce * t;
		}
	}
	// output tree
	vector<int> MTVertaces;
	for(int vertex = 1; vertex<=numOfV; vertex++) 
		if(vertex_dset.find(vertex) == rootOfSourse) MTVertaces.push_back(vertex);
	if(id > requests.size())
		forest.trees.push_back({ MTVertaces, MTEdges, s, id, ct}),
		requests[id] = {id, s, t, (MTVertaces.size() == numOfV), false};
	else
		*(requests[id].MT) = { MTVertaces, MTEdges, s, id, ct};
	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	/* Write your code here. */
	// stop tree
	int idx = id - 1;
	for(auto t_edge: MTidForest.trees[idx].E)
		edgesMap[t_edge.vertex[0]][t_edge.vertex[1]]->b += requests[idx].t;
	MTidForest.trees[idx] = { { }, { }, requests[idx].s, id, 0};
	requests[idx].isstoped = true;
	// connect other partial tree
	for(idx = 0; idx < MTidForest.size; idx++)
	{
		if(requests[idx].isFull || requests[idx].isstoped) continue;
		// init
		VertexDisjointSet vertex_dset(numOfV, MTidForest.trees[idx].V);
		vector<graphEdge*> new_MTEdges_G;
		EdgePQ edge_pq;
		for(int i=0; i<G.E.size(); i++) if(G.E[i].b >= requests[idx].t) edge_pq.push(&G.E[i]);
		// spanning tree
		while (!edge_pq.empty() && vertex_dset.numOfRoot() > 1)
		{
			graphEdge *newEdge = edge_pq.top();
			edge_pq.pop();
			if(vertex_dset.find(newEdge->vertex[0]) != vertex_dset.find(newEdge->vertex[1]))
			{
				new_MTEdges_G.push_back(newEdge);
				vertex_dset.unionSets(newEdge->vertex[0], newEdge->vertex[1]);
			}
		}
		// deal with effect of traffic
		int addintional_ct = 0, rootOfSourse = vertex_dset.find(requests[idx].s);
		vector<treeEdge> MTEdges;
		for(auto edge: new_MTEdges_G)
		{
			if(vertex_dset.find(edge->vertex[0]) == rootOfSourse) // exclude unconnected edges
			{
				MTEdges.push_back( { edge->vertex[0], edge->vertex[1]} );
				edge->b -= requests[idx].t;
				addintional_ct += edge->ce * requests[idx].t;
			}
		}
		// output tree
		Tree &MTid = MTidForest.trees[idx];
		MTidForest.trees[idx].V.clear();
		for(int vertex = 1; vertex<=numOfV; vertex++)
			if(vertex_dset.find(vertex) == rootOfSourse) MTidForest.trees[idx].V.push_back(vertex);
	}
	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	for(auto tree: MTidForest.trees) 
	tree = {{}, {}, tree.s, tree.id, 0};
	for(int i=0; i<G.E.size(); i++) G.E[i].b = G.E[i].be;
	for(int i=0; i<MTidForest.size; i++)
		if(!requests[i].isstoped)
			insert(MTidForest.trees[i].id, MTidForest.trees[i].s, { }, requests[i].t, G, MTidForest.trees[i]);
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

VertexDisjointSet::VertexDisjointSet(int numberOfV, vector<int> connectedVerteces) {
	d_set = new pair<int, int>[numberOfV + 1];
	numRoots = 0;
	if(connectedVerteces.size() < 2)
		for (int vertex = 1; vertex<=numberOfV; vertex++) d_set[vertex] = {-1, 0};
	else
	{
		for(int vertex = 1, nextConnectedVertaxId = 0; vertex<=numberOfV; vertex++)
		{
			if(vertex == connectedVerteces[nextConnectedVertaxId])
			{
				d_set[vertex] = { connectedVerteces[0], 0 };
				if(nextConnectedVertaxId < connectedVerteces.size() - 1) nextConnectedVertaxId++;
			}
			else
				d_set[vertex] = {-1, 1};
		}
		d_set[connectedVerteces[0]] = {-1, 1};
	}
	numRoots = numberOfV - (connectedVerteces.empty() ? 0 : connectedVerteces.size() - 1);
}

VertexDisjointSet::~VertexDisjointSet() 
{ 
	delete[] d_set; 
}

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
