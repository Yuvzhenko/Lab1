#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string trim(const string& source);

class Graph {
	vector<vector<int>> graph;
	bool directed;
public:
	Graph() { 
		cout << "Enter the amount of vertecies :: ";
		int n;
		cin >> n;
		graph.resize(n);
		cin.ignore();
		cout << "Is it directed (yes/no) :: ";
		string d;
		getline(cin, d);
		if (trim(d) == "yes")
			directed = true;
		else
			directed = false;
	}

	void createGraph();
	void printGraphList();
};