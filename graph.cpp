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
    if (matrix)
        for (int u = 0; u < vertices; u++) {
            if (graph[v][u] && !visited[u])
                DFS(u, visited);
        }
    else
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
    if (matrix) {
        gT.matrix = true;
        gT.graph.assign(vertices, vector<int>(vertices, 0));
        for (int i = 0; i < vertices; i++)
            for (int j = 0; j < vertices; j++)
                if (graph[i][j])
                    gT.graph[j][i] = 1;
    }
    else {
        gT.matrix = false;
        gT.graph.resize(vertices);
        for (int i = 0; i < vertices; i++) 
            for (int u : graph[i]) 
                gT.graph[u].push_back(i);
            
        
    }
    return gT;
}

void Graph::createGraphList() {
    matrix = false;
    graph.resize(vertices);

    cout << "Enter all the edges (v u) and -1 -1 when finished\n";
    int u, v;
    while (true) {
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
        for (auto i = graph[u].begin(); i != graph[u].end(); i++)
            if (*i == v) {
                graph[u].erase(i);
                removed = true;
                break;
            }

        if (!directed) 
            for (auto i = graph[v].begin(); i != graph[v].end(); i++)
                if (*i == v) {
                    graph[v].erase(i);
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
    if (matrix) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (graph[i][j]) {
                    start = i;
                    break;
                }
            }
            if (start != -1) break;
        }
    }
    else {
        for (int i = 0; i < vertices; i++) {
            if (!graph[i].empty()) {
                start = i;
                break;
            }
        }
    }

    if (start == -1) {
        cout << "Graph has no edges.\n";
        return;
    }

    DFS(start, visited);

    for (int i = 0; i < vertices; i++) {
        bool hasEdges = false;
        if (matrix) {
            for (int j = 0; j < vertices; j++)
                if (graph[i][j]) hasEdges = true;
        }
        else hasEdges = !graph[i].empty();

        if (hasEdges && !visited[i]) {
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
    if (matrix) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (graph[i][j]) {
                    start = i;
                    break;
                }
            }
            if (start != -1) break;
        }
    }
    else {
        for (int i = 0; i < vertices; i++) {
            if (!graph[i].empty()) {
                start = i;
                break;
            }
        }
    }
    if (start == -1) {
        cout << "Graph is not Connected.\n";
        return;
    }

    DFS(start, visited);
    for (bool v : visited)
        if (!v) {
            cout << "Graph is not Connected.\n";
            return;
        }

    Graph gT = getTranspose();
    for (int i = 0; i < visited.size(); i++)
        visited[i] = false;
    gT.DFS(start, visited);

    for (bool v : visited)
        if (!v) {
            cout << "Graph is not Connected.\n";
            return;
        }

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
        if (matrix) {
            for (int v = 0; v < vertices; v++) {
                if (graph[u][v] && !visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    queue.push_back(v);
                }
            }
        }
        else {
            for (int v : graph[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    queue.push_back(v);
                }
            }
        }
    }
    if (dist[end] == -1)
        cout << "Can't find a way between vertices\n";
    else
        cout << "Distance between the vertices is " << dist[end] << endl;
}