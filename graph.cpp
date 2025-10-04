#include "graph.h"
#include <iostream>
#include <string>

using namespace std;

string trim(const string& source) {
    string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}

void Graph::createGraph() {
    cout << "Enter all the edges (v u) and stop when finished\n";
    string u, v;
    int U, V;
    while (true) {
        getline(cin, u);
        getline(cin, v);
        if (trim(u) == "stop")
            break;
        U = stoi(u);
        V = stoi(v);
        graph[U].push_back(V);
    }
}
/*
void Graph::printGraphList() {
    for (int i = 0; i < graph.size(); i++) {
        std::cout << i << ": ";
        for (int j = 0; j < graph[i].size;) {

        }
        std::cout << "\n";
    }
}*/