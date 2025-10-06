#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string trim(const string& source);

class Graph {
	vector<vector<int>> graph;
	bool directed;
	bool matrix;
	int vertices;
public:
	Graph(int v = 0, bool dir = false) {
		vertices = v;
		directed = dir;
	}

	void createGraphList();		
	void createGraphMatrix();	
	void printGraphList();		
	void printGraphMatrix();	
	void addEdge();				
	void removeEdge();			
	void isConnected();			
	void distanceBetween();		

private:
	void DFS(int v, vector<bool>& visited);
	void isStronglyConnected();
	bool checkVertices(int &u, int &v);
	Graph getTranspose();
};