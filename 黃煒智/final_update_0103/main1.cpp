/***********************************************/
//         This .cpp is for Problem1           //
/***********************************************/
#include <iostream>
#include "basicDS.h"
#include "Problem1.cpp"
#include <fstream>
#include <string>
#include <queue>

void graph_input(Graph& G) {
	int vertex_num, edge_num;
	cin >> vertex_num >> edge_num;

	for (int i = 1; i <= vertex_num; i++) {
		G.V.push_back(i);
	}

	for (int i = 0; i < edge_num; i++) {
		graphEdge gE;
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		gE.vertex[0] = a;
		gE.vertex[1] = b;
		gE.b = c;
		gE.be = c;
		gE.ce = d;
		G.E.push_back(gE);
	}
}

void insert_input(int& id, int& s, int& t, Set& D, string input_string) {

	queue<string> q;

	while (1) {
		q.push(input_string.substr(0, input_string.find(" ")));
		input_string = input_string.substr(input_string.find(" ") + 1, input_string.length());

		if (input_string.find(" ") == -1) {
			q.push(input_string);
			break;
		}
	}

	q.pop();
	id = stoi(q.front());
	q.pop();
	s = stoi(q.front());
	q.pop();
	q.pop();

	while (1) {
		string tmp_s = q.front();
		q.pop();
		if (tmp_s == "}") { break; }
		int tmp_i = stoi(tmp_s);
		D.destinationVertices.push_back(tmp_i);
		D.size++;
	}

	t = stoi(q.front());
	q.pop();
}

int stop_input(string input_string) {
	string tmp_s = input_string.substr(input_string.find(" ") + 1, input_string.length());
	return stoi(tmp_s);
}

int main(void) {

	Graph G;
	Tree T;
	Forest F;
	graph_input(G);

	Problem1 P1(G);

	string input_string;
	while (getline(cin, input_string)) {
		if (input_string[0] == 'i') {
			int id, s, t;
			Set D;
			insert_input(id, s, t, D, input_string);
			P1.insert(id, s, D, t, G, T);
			F.trees.push_back(T);
			F.size++;
		}
		else if (input_string[0] == 's') {
			int id = stop_input(input_string);
			P1.stop(id, G, F);
		}
		else if (input_string[0] == 'r') {
			P1.rearrange(G, F);
		}
		for(auto it=G.E.begin();it!=G.E.cend();it++){
			cout<<"{"<<(it->vertex[0])<<", "<<(it->vertex[1])<<"}"<<"bandwidth = "<<(it->b)<<", cost = "<<(it->ce)<<endl;
		}
		if(F.size){
			for(auto it=--F.trees.cend();it!=F.trees.cend();it++){
				cout<<"tree id = " << it->id<<" root = " << it->s << " bandwidth cost = " << it->ct<<endl;
				for(auto itn=it->E.begin(); itn!=it->E.cend();itn++){
					cout<<"{ "<<itn->vertex[0]<<" "<<itn->vertex[1]<<" }";
				}
				cout<<endl;
			}
		}
	}
	return 0;
}
