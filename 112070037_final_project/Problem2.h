#ifndef PROBLEM2_H
#define PROBLEM2_H

#include "basicDS.h"
#include <map>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

struct request;
class VertexDisjointSet;
class Problem2 {
public:
	Problem2(Graph G);  //constructor
	~Problem2();        //destructor
	bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);
    static map<int, int> vertexDegreeMap;
private:
    void updateVertiexDegreeMap();
    bool findUsefulEdge(int s, set<int> &D, map<int, vector<int>> &MTEdgeMap, vector<graphEdge*> &usefulMTEdge, set<int>* checkedVertice = nullptr);
    Graph graph;
    int numOfV;
    map<int, map<int, graphEdge*>> edgesMap;
    map<int, request> requests;
    inline int square(int x) { return x * x; }
};
map<int, int> Problem2::vertexDegreeMap;

class CompareEdge
{
public:
    bool operator()(graphEdge *lhs, graphEdge *rhs) 
    { return computePoint(lhs) > computePoint(rhs); }
    int computePoint(graphEdge *edge);
    inline int square(int x) { return x * x; }
};

class CompareTree
{
public:
    bool operator()(const Tree* lhs, const Tree* rhs) const { return lhs->id < rhs->id; }
};

struct request
{
    int id, s, t;
    bool isFull;
    Tree MT;
    vector<int> D;
};

class VertexDisjointSet {
public:
    VertexDisjointSet(int numberOfV, vector<int> connectedVerteces = { });
	~VertexDisjointSet();
    int find(int x);
    void unionSets(int x, int y);
	int numOfRoot() { return numRoots; };
private:
    pair<int, int>* d_set;
	int numRoots;
};


#endif