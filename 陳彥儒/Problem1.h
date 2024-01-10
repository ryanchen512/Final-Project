#ifndef PROBLEM1_H
#define PROBLEM1_H

#include "basicDS.h"
#include <map>
#include <queue>

using namespace std;

struct request;
class VertexDisjointSet;

class Problem1 {
public:
	Problem1(Graph G);     //constructor
	~Problem1();        //destructor
	void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);
private:
    int numOfV;
    vector<request> requestsMap;
    map<int, graphEdge*> edgesMap;
    void spanningTree(Graph &G, Tree &tree, VertexDisjointSet &vertex_dset, vector<treeEdge> &MTedges, request r);
};

class CompareEdge
{
public:
    bool operator()(graphEdge *lhs, graphEdge *rhs) { return (lhs->ce > rhs->ce); }
};

struct request
{
    int id, s, t;
};

class VertexDisjointSet {
public:
    VertexDisjointSet(int numOfV);
	~VertexDisjointSet();
    int find(int x);
    void unionSets(int x, int y);
	int numOfRoot() { return numRoots; };
private:
    pair<int, int>* d_set;
	int numRoots;
};

#endif
