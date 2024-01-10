#include<iostream>
#include "basicDS.h"
#include <queue>
using namespace std;

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

bool finddis(int now, Set D);

struct cmp{
	bool operator()(const graphEdge* l, const graphEdge* r){
		return l->ce > r->ce;
	}
};

class Information{
	public:
		bool stop=false;
		int s, t;
		vector<int> all;
};

Information inf[100005];

class Problem1 {
public:

	Problem1(Graph G);  //constructor
	~Problem1();        //destructor
	void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);

private:
	int num;
};

Problem1::Problem1(Graph G) {
	/* Write your code here. */

}

Problem1::~Problem1() {
	/* Write your code here. */

}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	inf[id].s=s;
	inf[id].t=t;
	inf[id].all=D.destinationVertices;

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

	delete [] arr;

	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */
	for(auto it=MTidForest.trees.begin();it!=MTidForest.trees.cend();it++){
		if(it->id==id){
			inf[id].stop=true;
			auto nowtree=MTidForest.trees[id-1];
			for(auto ittre=nowtree.E.begin();ittre!=nowtree.E.cend();ittre++){
				for(auto itgre=G.E.begin();itgre!=G.E.cend();itgre++){
					if(ittre->vertex[0]==itgre->vertex[0]&&ittre->vertex[1]==itgre->vertex[1]){
						itgre->b+=inf[id].t;
						//cout<<ittre->vertex[0]<<" "<<ittre->vertex[1]<<" "<<itgre->b;
						break;
					}
				}
				//cout<<endl;
			}
			
			for(auto nowtree=MTidForest.trees.begin();nowtree!=MTidForest.trees.cend();nowtree++){
				//cout<<inf[nowtree->id].stop<<nowtree->V.size()<<inf[nowtree->id].all.size()<<endl;
				if(inf[nowtree->id].stop || nowtree->V.size()==inf[nowtree->id].all.size()) continue;

				auto MTid=nowtree;
				int s=MTid->s;
				int t=inf[MTid->id].t;
				auto dis=inf[MTid->id].all;
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
				delete [] arr;
			}
			break;
		}
	}

	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	int numtree=MTidForest.trees.size();
	for(auto it:G.E){
		for(auto &gE:G.E){
			gE.b=gE.be;
		}
	}
	for(auto nowtree=MTidForest.trees.begin();nowtree!=MTidForest.trees.cend();nowtree++){
		nowtree->V.clear();
		nowtree->E.clear();
		nowtree->ct=0;
	}


	for(auto nowtree=MTidForest.trees.begin();nowtree!=MTidForest.trees.cend();nowtree++){
		//cout<<inf[nowtree->id].stop<<nowtree->V.size()<<inf[nowtree->id].all.size()<<endl;
		if(inf[nowtree->id].stop) continue;
		auto MTid=nowtree;
		int s=MTid->s;
		int t=inf[MTid->id].t;
		auto dis=inf[MTid->id].all;
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
		delete [] arr;
	}
	
	return;
}

