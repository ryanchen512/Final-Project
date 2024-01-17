#include "basicDS.h"
#include "Problem1.h"

using namespace std;
using EdgePQ = priority_queue<graphEdge*, vector<graphEdge*>, CompareEdge>;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

Problem1::Problem1(Graph G) {
	/* Write your code here. */
	graph = G;
	numOfV = graph.V.size(); 
	for(int i=0; i<G.E.size(); i++) 
		edgesMap[graph.E[i].vertex[0]][graph.E[i].vertex[1]] = &graph.E[i];
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
	for(int i=0; i<graph.E.size(); i++) if(graph.E[i].b >= t) edge_pq.push(&graph.E[i]);
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
	if(requests.find(id) == requests.end())
	{
		requests[id] = {id, s, t, (MTVertaces.size() == numOfV), {MTVertaces, MTEdges, s, id, ct} };
		MTid = requests[id].MT;
	}
	else
		requests[id].MT = {MTVertaces, MTEdges, s, id, ct};
	G = graph;
	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	/* Write your code here. */
	// stop tree
	for(auto t_edge: requests[id].MT.E)
		edgesMap[t_edge.vertex[0]][t_edge.vertex[1]]->b += requests[id].t;
	requests.erase(id);
	for(auto it = requests.begin(); it != requests.end(); ++it)
	{
		auto request = it->second;
		cout << request.id;
		// init
		VertexDisjointSet vertex_dset(numOfV, request.MT.V);
		vector<graphEdge*> new_MTEdges_G;
		EdgePQ edge_pq;
		for(int i=0; i<graph.E.size(); i++) if(graph.E[i].b >= request.t) edge_pq.push(&graph.E[i]);
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
		int addintional_ct = 0 , rootOfSourse = vertex_dset.find(request.s);
		for(auto edge: new_MTEdges_G)
		{
			if(vertex_dset.find(edge->vertex[0]) == rootOfSourse) // exclude unconnected edges
			{
				request.MT.E.push_back( { edge->vertex[0], edge->vertex[1]} );
				edge->b -= request.t;
				addintional_ct += edge->ce * request.t;
			}
		}
		// output tree
		for(int vertex = 1; vertex<=numOfV; vertex++) 
			if(vertex_dset.find(vertex) == rootOfSourse) request.MT.V.push_back(vertex);
		sort(request.MT.V.begin(), request.MT.V.end());
		if(new_MTEdges_G.size() != 0) MTidForest.trees.push_back(request.MT);
	}
	MTidForest.size = MTidForest.trees.size();
	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	for(int i=0; i<graph.E.size(); i++) graph.E[i].b = graph.E[i].be;
	for(auto it = requests.begin(); it != requests.end(); ++it)
	{
		auto request = it->second;
		it->second.MT = {{}, {}, it->second.s, it->second.id, 0};
		insert(request.id, request.s, { }, request.t, graph, request.MT);
		MTidForest.trees.push_back(request.MT);
	}
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
	// cout << connectedVerteces.size() << endl;
	// for(int i=0; i<numberOfV+1; i++) cout <<  d_set[i].first << " " << d_set[i].second << endl;
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
