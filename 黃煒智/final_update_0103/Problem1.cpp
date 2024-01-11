#include<iostream>
#include "basicDS.h"
#include <queue>
#include <algorithm>
using namespace std;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */


struct cmp{
	bool operator()(const graphEdge* l, const graphEdge* r){
		return l->ce > r->ce;
	}
};

class Information{
	public:
		vector<int> V;
		vector<treeEdge> E;
		int s;          // source node
		int id;         // corresponding multicast request id
		int ct;         // transmission cost of multicast tree
		bool stop=false;
		int t;
		vector<int> all;
};

struct FInformation{
	int size = 0;
	vector<Information> trees;
};

bool cmp1(const Information l, const Information r) {
	return l.id < r.id;
}

class Problem1 {
public:

	Problem1(Graph G);  //constructor
	~Problem1();        //destructor
	void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);

private:
	FInformation F;
};

Problem1::Problem1(Graph G) {
	/* Write your code here. */

}

Problem1::~Problem1() {
	/* Write your code here. */

}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	Information MTidI;

	MTid.id=id;
	MTid.s=s;
	MTid.ct=0;
	MTid.V.clear();
	MTid.E.clear();
	bool* arr=new bool[100001]();

	/* Write your code here. */
	priority_queue<graphEdge*, vector<graphEdge*>, cmp> pq;

	int now=s;
	MTid.V.push_back(now);
	arr[s]=true;
	for(auto it:G.E){
		for(auto &gE:G.E){
			if(gE.vertex[0]==now&&!arr[gE.vertex[1]]){
				pq.push(&gE);
			}
			if(gE.vertex[1]==now&&!arr[gE.vertex[0]]){
				pq.push(&gE);
			}
		}
		while(!pq.empty()){
			//cout<<"{ "<<pq.top()->vertex[0]<<" "<<pq.top()->vertex[1]<<" }";
			auto gE=pq.top();
			if(gE->b>=t){
				if(!(arr[gE->vertex[1]])){
					gE->b-=t;
					treeEdge Te;
					Te.vertex[0]=gE->vertex[0];
					Te.vertex[1]=gE->vertex[1];
					arr[gE->vertex[1]]=true;
					MTid.E.push_back(Te);
					MTid.ct+=(gE->ce);
					//cout<<pq.top()->ce<<endl;
					now=gE->vertex[1];
					MTid.V.push_back(now);
					pq.pop();
					break;
				}
				else if(!(arr[gE->vertex[0]])){
					gE->b-=t;
					treeEdge Te;
					Te.vertex[0]=gE->vertex[0];
					Te.vertex[1]=gE->vertex[1];
					arr[gE->vertex[0]]=true;
					MTid.E.push_back(Te);
					MTid.ct+=(gE->ce);
					//cout<<pq.top()->ce<<endl;
					now=gE->vertex[0];
					MTid.V.push_back(now);
					pq.pop();
					break;
				}
			}
			pq.pop();
		}
	}
	
	MTidI.V=MTid.V;
	MTidI.E=MTid.E;
	MTidI.s=s;
	MTidI.id=id;
	MTidI.ct=MTid.ct;
	MTidI.t=t;
	MTidI.all=D.destinationVertices;
	
	F.trees.push_back(MTidI);
	F.size++;

	delete [] arr;

	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */
	MTidForest.size=0;
	MTidForest.trees.clear();
	sort(F.trees.begin(), F.trees.end(), cmp1);

	for(auto it=F.trees.begin();it!=F.trees.cend();it++){
		if(it->id==id){
			it->stop=true;
			Information* nowtree;
			for(auto &ittre:F.trees){
				if(ittre.id==id){
					nowtree=&ittre;
					break;
				}
			}
			for(auto ittre=nowtree->E.begin();ittre!=nowtree->E.cend();ittre++){
				for(auto itgre=G.E.begin();itgre!=G.E.cend();itgre++){
					if(ittre->vertex[0]==itgre->vertex[0]&&ittre->vertex[1]==itgre->vertex[1]){
						itgre->b+=(nowtree->t);
						//cout<<ittre->vertex[0]<<" "<<ittre->vertex[1]<<" "<<itgre->b;
						break;
					}
				}
				//cout<<endl;
			}
			nowtree->V.clear();
			nowtree->E.clear();
			nowtree->ct=0;
			
			for(auto nowtree=F.trees.begin();nowtree!=F.trees.cend();nowtree++){
				//cout<<inf[nowtreeid].stop<<MTidForest.trees[nowtreeid-1].V.size()<<inf[nowtreeid].all.size()<<endl;
				if(nowtree->stop || nowtree->V.size()==nowtree->all.size()) continue;

				bool change=false;

				auto MTid=nowtree;
				int s=MTid->s;
				int t=MTid->t;
				auto dis=MTid->all;
				bool* arr=new bool[100001]();

				priority_queue<graphEdge*, vector<graphEdge*>, cmp> pq;
				int now=s;

				for(auto iter:MTid->V){
					arr[iter]=true;
				}

				for(auto iter:MTid->V){
					for(auto &gE:G.E){
						if(gE.vertex[0]==iter&&!arr[gE.vertex[1]]){
							pq.push(&gE);
						}
						if(gE.vertex[1]==iter&&!arr[gE.vertex[0]]){
							pq.push(&gE);
						}
					}
				}
				
				for(auto iter:G.E){
					for(auto &gE:G.E){
						if(gE.vertex[0]==now&&!arr[gE.vertex[1]]){
							pq.push(&gE);
						}
						if(gE.vertex[1]==now&&!arr[gE.vertex[0]]){
							pq.push(&gE);
						}
					}
					while(!pq.empty()){
						//cout<<"{ "<<pq.top()->vertex[0]<<" "<<pq.top()->vertex[1]<<" }";
						auto gE=pq.top();
						if(gE->b>=t){
							//cout<<gE->ce<<" ";
							if(!(arr[gE->vertex[1]])){
								gE->b-=t;
								treeEdge Te;
								Te.vertex[0]=gE->vertex[0];
								Te.vertex[1]=gE->vertex[1];
								arr[gE->vertex[1]]=true;
								MTid->E.push_back(Te);
								MTid->ct+=(gE->ce);
								//cout<<pq.top()->ce<<endl;
								now=gE->vertex[1];
								MTid->V.push_back(now);
								change=true;
								pq.pop();
								break;
							}
							else if(!(arr[gE->vertex[0]])){
								gE->b-=t;
								treeEdge Te;
								Te.vertex[0]=gE->vertex[0];
								Te.vertex[1]=gE->vertex[1];
								arr[gE->vertex[0]]=true;
								MTid->E.push_back(Te);
								MTid->ct+=(gE->ce);
								//cout<<pq.top()->ce<<endl;
								now=gE->vertex[0];
								MTid->V.push_back(now);
								change=true;
								pq.pop();
								break;
							}
						}
						pq.pop();
					}
				}
				
				delete [] arr;
				if(change){
					Tree T;
					T.V=MTid->V;
					T.E=MTid->E;
					T.s=s;
					T.id=MTid->id;
					T.ct=MTid->ct;	
					MTidForest.size++;
					MTidForest.trees.push_back(T);
				}
			}
			break;
		}
	}

	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */
	MTidForest.size=0;
	MTidForest.trees.clear();
	sort(F.trees.begin(), F.trees.end(), cmp1);

	/* Write your code here. */
	

	int numtree=F.trees.size();

	for(auto &gE:G.E){
		gE.b=gE.be;
	}

	for(auto nowtree=F.trees.begin();nowtree!=F.trees.cend();nowtree++){
		nowtree->V.clear();
		nowtree->E.clear();
		nowtree->ct=0;
	}


	for(auto nowtree=F.trees.begin();nowtree!=F.trees.cend();nowtree++){
		//cout<<inf[nowtree->id].stop<<nowtree->V.size()<<inf[nowtree->id].all.size()<<endl;
		if(nowtree->stop) continue;
		auto MTid=nowtree;
		int s=MTid->s;
		int t=MTid->t;
		auto dis=MTid->all;
		bool* arr=new bool[100001]();

		priority_queue<graphEdge*, vector<graphEdge*>, cmp> pq;
		int now=s;
		MTid->V.push_back(now);
		arr[s]=true;

		for(auto iter:G.E){
			for(auto &gE:G.E){
				if(gE.vertex[0]==now&&!arr[gE.vertex[1]]){
					pq.push(&gE);
				}
				if(gE.vertex[1]==now&&!arr[gE.vertex[0]]){
					pq.push(&gE);
				}
			}
			while(!pq.empty()){
				//cout<<"{ "<<pq.top()->vertex[0]<<" "<<pq.top()->vertex[1]<<" }";
				auto gE=pq.top();
				if(gE->b>=t){
					//cout<<gE->ce<<" ";
					if(!(arr[gE->vertex[1]])){
						gE->b-=t;
						treeEdge Te;
						Te.vertex[0]=gE->vertex[0];
						Te.vertex[1]=gE->vertex[1];
						arr[gE->vertex[1]]=true;
						MTid->E.push_back(Te);
						MTid->ct+=(gE->ce);
						//cout<<pq.top()->ce<<endl;
						now=gE->vertex[1];
						MTid->V.push_back(now);
						pq.pop();
						break;
					}
					else if(!(arr[gE->vertex[0]])){
						gE->b-=t;
						treeEdge Te;
						Te.vertex[0]=gE->vertex[0];
						Te.vertex[1]=gE->vertex[1];
						arr[gE->vertex[0]]=true;
						MTid->E.push_back(Te);
						MTid->ct+=(gE->ce);
						//cout<<pq.top()->ce<<endl;
						now=gE->vertex[0];
						MTid->V.push_back(now);
						pq.pop();
						break;
					}
				}
				pq.pop();
			}
		}

		Tree T;
		T.V=MTid->V;
		T.E=MTid->E;
		T.s=s;
		T.id=MTid->id;
		T.ct=MTid->ct;	
		MTidForest.size++;
		MTidForest.trees.push_back(T);

		delete [] arr;
	}
	
	return;
}

