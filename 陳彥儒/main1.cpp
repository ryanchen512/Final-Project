/***********************************************/
//         This .cpp is for Problem1           //
/***********************************************/
#include <iostream>
#include "basicDS.h"
#include "Problem1.cpp"
#include <chrono>
#include <fstream>
#include <string>
#include <queue>

using namespace std;
using namespace std::chrono;

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

bool isdebug = false;
int command = 0;

int main(void) {
	auto start = high_resolution_clock::now();
	freopen("Problem1_test_case.txt", "r", stdin);
	//freopen("sample_output.txt", "w", stdout);

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
			if(isdebug)
			{
				command++;
				cout << "\033[1;33mCommand " << command << "  insert" << "\033[0m" << endl;
				cout << "request id: " << T.id << endl;
				for(auto edge: T.E)
				{
					cout << "{" << edge.vertex[0] << ", " << edge.vertex[1] << "}, ";
				}
				cout << "total cost is " << T.ct << endl;
			}
		}
		else if (input_string[0] == 's') {
			int id = stop_input(input_string);
			P1.stop(id, G, F);
			if(isdebug)
			{
				command++;
				cout << "\033[1;33mCommand " << command << "  stop" << "\033[0m" << endl;
				for(auto tree: F.trees)
				{
					cout << "request id: " << tree.id << endl;
					for(auto edge: tree.E)
					{
						cout << "{" << edge.vertex[0] << ", " << edge.vertex[1] << "}, ";
					}
					cout << "total cost is " << tree.ct << endl;
				}
				if(F.trees.size() == 0) cout << "No tree be modified" << endl;
			}
		}
		else if (input_string[0] == 'r') {
			P1.rearrange(G, F);
			if(isdebug)
			{
				command++;
				cout << "\033[1;33mCommand " << command << "  rearrange" << "\033[0m" << endl;
				for(auto tree: F.trees)
				{
					cout << "request id: " << tree.id << endl;
					for(auto edge: tree.E)
					{
						cout << "{" << edge.vertex[0] << ", " << edge.vertex[1] << "}, ";
					}
					cout << "total cost is " << tree.ct << endl;
				}
			}
		}
	}
	auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
	return 0;
}
