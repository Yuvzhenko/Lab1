#include "graph.h"


using namespace std;

string trim(const string& source) {
    string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}

void Graph::createGraph() {
    cout << "Enter all the edges (v u) and -1 -1 when finished\n";
    int u, v;
    while (true) {
        cin >> u;
        cin >> v;
        if (u == -1 and v == -1)
            break;
        if (u >= 0 && u < graph.size() && v >= 0 && v < graph.size()) {
            graph[u].push_back(v);
            if(!directed)
                graph[v].push_back(u);
        }
        else {
            cout << "Invalid vertex!\n";
        }
    }
}
void Graph::printGraphList() {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ": ";
        for (int v : graph[i]) {
            cout << v << " ";
        }
        cout << "\n";
    }
}