#ifndef PROBLEM2_H
#define PROBLEM2_H

#include "basicDS.h"
#include <map>
#include <queue>
#include <algorithm>

using namespace std;
#define SQUARE(x) ((x) * (x))

struct request;
class VertexDisjointSet;
class Problem2 {
public:
	Problem2(Graph G);  //constructor
	~Problem2();        //destructor
	bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);
    static map<int, int> vertiexDegreeMap;
private:
    void updateVertiexDegreeMap();
    Graph graph;
};

class CompareEdge
{
public:
    bool operator()(graphEdge *lhs, graphEdge *rhs) 
    { return computePoint(lhs) > computePoint(rhs); }
    int computePoint(graphEdge *edge)
    { 
        auto degreeMap = Problem2::vertiexDegreeMap;
        return square(degreeMap[edge->vertex[0]]) + square(degreeMap[edge->vertex[0]]) - edge->ce;
    }
    inline int square(int x) { return x * x; }
};

class CompareTree
{
public:
    bool operator()(const Tree* lhs, const Tree* rhs) const { return lhs->id < rhs->id; }
    //bool operator()(Tree *lhs, Tree *rhs) { return (lhs->id < rhs->id); }
};

struct request
{
    int id, s, t;
    bool isFull;
    Tree MT;
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