#pragma once
#include <iostream>

#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>

#include <fstream>

using namespace std;

string trim(const string& source);

class Graph {
protected:
	vector<vector<int>> graph;
	fstream file;
	bool directed;
	bool matrix;
	int vertices;
public:

	void setVert(int v) { vertices = v; }
	void setDir(bool dir) { directed = dir; }
	bool getMatrix() { return matrix; }

	void createGraphList();		
	void createGraphMatrix();	
	void printGraphList();		
	void printGraphMatrix();	
	void addEdge();				
	void removeEdge();			
	void isConnected();			
	void distanceBetween();	
	bool loadFromFile();

protected:
	void getGraphData(int& v, bool& dir);
	void DFS(int v, vector<bool>& visited);
	void isStronglyConnected();
	const void matrixToFile();
	const void listToFile();
	bool checkVertices(int &u, int &v);
	Graph getTranspose();
};