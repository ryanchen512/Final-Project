#include "basicDS.h"
#include "Problem2.h"

using namespace std;
using EdgePQ = priority_queue<graphEdge*, vector<graphEdge*>, CompareEdge>;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

Problem2::Problem2(Graph G) {
	/* Write your code here. */
	graph = G;
	numOfV = graph.V.size(); 
	for(int i=0; i<G.E.size(); i++) 
	{
		edgesMap[graph.E[i].vertex[0]][graph.E[i].vertex[1]] = &graph.E[i];
		edgesMap[graph.E[i].vertex[1]][graph.E[i].vertex[0]] = &graph.E[i];
	}
}

Problem2::~Problem2() {
	/* Write your code here. */

}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	/* Write your code here. */
	/* You should return true or false according the insertion result */
	// init
	updateVertiexDegreeMap();
	VertexDisjointSet vertex_dset(numOfV);
	vector<graphEdge*> MTEdges_G;
	EdgePQ edge_pq;
	for(int i=0; i<graph.E.size(); i++) 
	{	
		if(graph.E[i].b >= t) edge_pq.push(&graph.E[i]);
	}
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
	// check tree is full
	for(auto vertex: D.destinationVertices)
		if(vertex_dset.find(vertex) != vertex_dset.find(s)) 
		{
			requests[id] = {id, s, t, false, { { }, { }, s, id, 0}, D.destinationVertices };
			return false;
		}
	// RemoveUselessEdge
	map<int, vector<int>> MTEdgeMap;
	vector<graphEdge*> usefulMTEdge;
	for(int i=0; i<MTEdges_G.size(); i++) 
	{
		MTEdgeMap[MTEdges_G[i]->vertex[0]].push_back(MTEdges_G[i]->vertex[1]);
		MTEdgeMap[MTEdges_G[i]->vertex[1]].push_back(MTEdges_G[i]->vertex[0]);
	}
	set<int> Dset;
	for(auto DEdge: D.destinationVertices) Dset.insert(DEdge);
	findUsefulEdge(s, Dset, MTEdgeMap, usefulMTEdge);
	// deal with effect of traffic
	int ct = 0;
	for(int i=0; i<usefulMTEdge.size(); i++) 
	{
		usefulMTEdge[i]->b -= t;
		ct += usefulMTEdge[i]->ce * t;
	}
	// output tree
	vector<treeEdge> usefulMTEdge_T;
	for(auto edge: usefulMTEdge) usefulMTEdge_T.push_back({edge->vertex[0], edge->vertex[1]});
	if(requests.find(id) == requests.end())
		requests[id] = {id, s, t, true, {D.destinationVertices , usefulMTEdge_T, s, id, ct}, D.destinationVertices};
	else
		requests[id].MT = {D.destinationVertices, usefulMTEdge_T, s, id, ct};
	MTid = requests[id].MT;
	G = graph;
	return true;
}

bool Problem2::findUsefulEdge(int s, set<int> &D, map<int, vector<int>> &MTEdgeMap, vector<graphEdge*> &usefulMTEdge, set<int>* checkedVertice)
{
	bool needToDeleteSet = false, isUseful = false;
	if(checkedVertice == nullptr) checkedVertice  = new set<int>, needToDeleteSet = true;
	checkedVertice->insert(s);
	for(auto v: MTEdgeMap[s])
	{
		if(checkedVertice->find(v) == checkedVertice->end())
		if(findUsefulEdge(v, D, MTEdgeMap, usefulMTEdge, checkedVertice))
		{
			isUseful = true;
			usefulMTEdge.push_back(edgesMap[s][v]);
		}
	}
	if(D.find(s) != D.end()) return true;
	if(MTEdgeMap[s].size() == 0) return false;
	if(needToDeleteSet) delete checkedVertice;
	return isUseful;
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	/* Write your code here. */
	// stop tree
	for(auto t_edge: requests[id].MT.E)
		edgesMap[t_edge.vertex[0]][t_edge.vertex[1]]->b += requests[id].t;
	requests.erase(id);
	// connect other partial tree
	for(auto it = requests.begin(); it != requests.end(); ++it)
	{
		auto request = it->second;
		if(request.isFull) continue;
		// init
		updateVertiexDegreeMap();
		VertexDisjointSet vertex_dset(numOfV);
		vector<graphEdge*> MTEdges_G;
		EdgePQ edge_pq;
		for(int i=0; i<graph.E.size(); i++) 
		{	
			if(graph.E[i].b >= request.t) edge_pq.push(&graph.E[i]);
		}
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
		// check tree is full
		bool isC = false;
		for(auto vertex: request.D)
			if(vertex_dset.find(vertex) != vertex_dset.find(request.s)) 
				isC = true;
		if(isC) continue;
		// RemoveUselessEdge
		map<int, vector<int>> MTEdgeMap;
		vector<graphEdge*> usefulMTEdge;
		for(int i=0; i<MTEdges_G.size(); i++) 
		{
			MTEdgeMap[MTEdges_G[i]->vertex[0]].push_back(MTEdges_G[i]->vertex[1]);
			MTEdgeMap[MTEdges_G[i]->vertex[1]].push_back(MTEdges_G[i]->vertex[0]);
		}
		set<int> Dset;
		for(auto DEdge:  request.D) Dset.insert(DEdge);
		findUsefulEdge(request.s, Dset, MTEdgeMap, usefulMTEdge);
		// deal with effect of traffic
		int ct = 0;
		for(int i=0; i<usefulMTEdge.size(); i++) 
		{
			usefulMTEdge[i]->b -= request.t;
			ct += usefulMTEdge[i]->ce * request.t;
		}
		// output tree
		vector<treeEdge> usefulMTEdge_T;
		for(auto edge: usefulMTEdge) usefulMTEdge_T.push_back({edge->vertex[0], edge->vertex[1]});
			request.MT = {request.D, usefulMTEdge_T, request.s, request.id, ct};
		MTidForest.trees.push_back(request.MT);
	}
	G = graph;
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	for(int i=0; i<graph.E.size(); i++) graph.E[i].b = graph.E[i].be;
	set<int> priorRequestId;
	for(auto it = requests.begin(); it != requests.end(); ++it)
	{
		auto request = it->second;
		if(request.isFull) continue;
		else priorRequestId.insert(request.id);
		it->second.MT = {{}, {}, it->second.s, it->second.id, 0};
		if(insert(request.id, request.s, {0, request.D}, request.t, graph, request.MT))
			MTidForest.trees.push_back(request.MT);
	}
	for(auto it = requests.begin(); it != requests.end(); ++it)
	{
		auto request = it->second;
		if(priorRequestId.find(request.id) != priorRequestId.end()) continue;
		it->second.MT = {{}, {}, it->second.s, it->second.id, 0};
		if(insert(request.id, request.s, {0, request.D}, request.t, graph, request.MT))
			MTidForest.trees.push_back(request.MT);
	}
	return;
}

void Problem2::updateVertiexDegreeMap()
{
	auto map = Problem2::vertexDegreeMap;
	for(int i=1; i<=numOfV; i++) map[i] = 0;
	for(auto edge: graph.E)
	{
		map[edge.vertex[0]] += square(edge.b);
		map[edge.vertex[1]] += square(edge.b);
	}
}


int CompareEdge::computePoint(graphEdge *edge)
{ 
	auto degreeMap = Problem2::vertexDegreeMap;
	return degreeMap[edge->vertex[0]] + degreeMap[edge->vertex[0]] - square(edge->ce);
}


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
