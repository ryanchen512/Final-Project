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
    Graph graph;
    int numOfV;
    map<int, map<int, graphEdge*>> edgesMap;
    map<int, request> requests;
    Forest forest;
};

class CompareEdge
{
public:
    bool operator()(graphEdge *lhs, graphEdge *rhs) { return (lhs->ce > rhs->ce); }
};

struct request
{
    int id, s, t;
    bool isFull, isstoped;
    Tree* MT;
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
