#include "graph.h"


using namespace std;

string trim(const string& source) {
    string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}

void Graph::DFS(int v, vector<bool>& visited) {
    visited[v] = true;
    for (int u : graph[v]) {
        if (!visited[u])
            DFS(u, visited);
    }
}

bool Graph::checkVertices(int &u, int &v) {
    cin >> u >> v;
    if (u == -1 && v == -1)
        return false; 

    if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
        cout << "Invalid vertex!\n";
        return checkVertices(u, v); 
    }

    return true;
}

Graph Graph::getTranspose() {
    Graph gT(vertices, directed);
    for (int i = 0; i < vertices; i++) {
        for (int u : graph[i]) {
            gT.graph[u].push_back(i);
        }
    }
    return gT;
}

void Graph::createGraphList() {
    matrix = false;
    graph.resize(vertices);

    cout << "Enter all the edges (v u) and -1 -1 when finished\n";
    int u, v;
    while (true) {
        cin >> u >> v;
        if (!checkVertices(u, v))
            break;

        graph[u].push_back(v);
        if(!directed)
            graph[v].push_back(u);
        
     
    }
}

void Graph::createGraphMatrix() {
    matrix = true;
    graph.assign(vertices, vector<int>(vertices, 0));

    cout << "Enter all the edges (v u) and -1 -1 when finished\n";
    int u, v;
    while (true) {
        cin >> u >> v;
        if (!checkVertices(u, v))
            break;

        graph[u][v] = 1;
        if (!directed)
            graph[v][u] = 1;
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

void Graph::printGraphMatrix() {
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph.size(); j++) {
            std::cout << graph[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void Graph::addEdge() {
    cout << "Enter the vertex to add (u v) or -1 -1 to exit\n";
    int u, v;
    if (!checkVertices(u, v))
        return;
    if (matrix){
        graph[u][v] = 1; 
        if(!directed) graph[v][u] = 1;
    }
    else {
        graph[u].push_back(v);
        if (!directed) graph[v].push_back(u);
    }
}

void Graph::removeEdge() {
    cout << "Enter the vertex to remove (u v) or -1 -1 to exit\n";
    int u, v;
    if (!checkVertices(u, v))
        return;
    bool removed = false;
    if (matrix) {
        graph[u][v] = 0;
        removed = true;
        if (!directed) graph[v][u] = 0;
    }
    else {
        for (auto i = 0; i < graph[u].size(); ++i) 
            if (graph[u][i] == v) {
                graph[u].erase(graph[u].begin() + i);
                removed = true;
                break;
            }

        if (!directed) 
            for (auto i = 0; i < graph[v].size(); ++i)
                if (graph[v][i] == u) {
                    graph[v].erase(graph[u].begin() + i);
                    break;
                }
    }
    if (!removed) cout << "Edge (" << u << " " << v << ") doesn't exist\n";
}

void Graph::isConnected() {
    if (directed)
        return isStronglyConnected();

    vector<bool> visited(vertices, false);

    int start = -1;
    for (int i = 0; i < vertices; i++) {
        if (!graph[i].empty()) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        cout << "Graph has no edges.\n";
        return;
    }

    DFS(start, visited);

    for (int i = 0; i < vertices; i++) {
        if (!graph[i].empty() && !visited[i]) {
            cout << "Graph is not Connected.\n";
            return;
        }
    }
    cout << "Graph is Connected.\n";
    return;
}

void Graph::isStronglyConnected() {
    vector<bool> visited(vertices, false);

    int start = -1;
    for (int i = 0; i < vertices; i++) {
        if (!graph[i].empty()) {
            start = i;
            break;
        }
    }
    if (start == -1) { cout << "Graph is not Connected.\n"; return; }

    DFS(start, visited);
    for (bool v : visited)
        if (!v) 
            { cout << "Graph is not Connected.\n"; return; };

    Graph gT = getTranspose();
    for (int i = 0; i < visited.size(); i++)
        visited[i] = false;
    gT.DFS(start, visited);

    for (bool v : visited)
        if (!v)
        { cout << "Graph is not Connected.\n"; return; }

    cout << "Graph is Connected.\n";
    return;
}

void Graph::distanceBetween() {
    cout << "Enter the vertex to find distance (u v) or -1 -1 to exit\n";
    int start, end;
    if (!checkVertices(start, end))
        return;

    vector<bool> visited(vertices, false);
    vector<int> dist(vertices, -1); 

    vector<int> queue;
    int front = 0;

    queue.push_back(start);
    visited[start] = true;
    dist[start] = 0;

    while (front < queue.size()) {
        int u = queue[front++];

        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                dist[v] = dist[u] + 1;
                queue.push_back(v);
            }
        }
    }
    if (dist[end] == -1)
        cout << "Can't find a way between vertices\n";
    else
        cout << "Distance between the vertices is " << dist[end] << endl;
    return;
}