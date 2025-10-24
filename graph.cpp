#include "graph.h"


using namespace std;

HANDLE console_textcolor = GetStdHandle(STD_OUTPUT_HANDLE);

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
    if (cin.fail()) {
        cin.clear();
        cin.ignore(32767, '\n');
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Invalid vertex!\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return checkVertices(u, v);
    }
    if (u == -1 && v == -1)
        return false; 

    if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Invalid vertex!\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return checkVertices(u, v); 
    }

    return true;
}

Graph Graph::getTranspose() {
    Graph gT;
    gT.setVert(vertices);
    gT.setDir(directed);
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

const void Graph::matrixToFile() {
    file.open("graph.txt", ios::out);
    if (!file.is_open()) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph wasn't saved to file\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return;
    }

    file << vertices << " " << directed << " " << matrix << "\n";

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++)
            file << graph[i][j] << " ";
        file << "\n";
    }
    file.close();
}

const void Graph::listToFile() {
    file.open("graph.txt", ios::out);
    if (!file.is_open()) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph wasn't saved to file\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return;
    }

    file << vertices << " " << directed << " " << matrix << "\n";

    for (int i = 0; i < vertices; i++) {
        file << i << ": ";
        for (int v : graph[i])
            file << v << " ";
        file << "\n";
    }
    file.close();
}

bool Graph::loadFromFile() {
    file.open("graph.txt", ios::in);
    if (!file.is_open()) {
        return false;
    }

    file >> vertices >> directed >> matrix;
    file.ignore();

    graph.clear();
    if (matrix) {
        graph.assign(vertices, vector<int>(vertices, 0));
        for (int i = 0; i < vertices; i++)
            for (int j = 0; j < vertices; j++)
                file >> graph[i][j];
    }
    else {
        graph.resize(vertices);
        string line;
        getline(file, line); 
        while (getline(file, line)) {
            if (trim(line).empty()) continue;

            size_t colon = line.find(':');
            if (colon == string::npos) continue;

            string vertexStr = line.substr(0, colon);
            int i = stoi(trim(vertexStr));  

            if (i >= (int)graph.size())
                graph.resize(i + 1);

            string rest = line.substr(colon + 1);
            stringstream ss(rest);
            int v;
            while (ss >> v) {
                if (v >= (int)graph.size())
                    graph.resize(v + 1);
                graph[i].push_back(v);
            }
        }
    }

    file.close();
    cout << "Your graph was loaded from file!\n";
    return true;
}

void Graph::getGraphData(int& v, bool& dir) {
    while (true) {
        cout << "Enter the amount of vertices :: ";
        cin >> v;
        if (cin.fail() || v < 1) {
            cin.clear();
            cin.ignore(32767, '\n');
            SetConsoleTextAttribute(console_textcolor, 4);
            cout << "Invalid vertex!\n";
            SetConsoleTextAttribute(console_textcolor, 0x0F);
        }
        else {
            cin.ignore();
            break;
        }
    }
    while (true) {
        cout << "Is it directed (yes/no) :: ";
        string d;
        getline(cin, d);
        if (trim(d) == "yes")
            dir = true;
        else
            if (trim(d) == "no")
                dir = false;
            else {
                SetConsoleTextAttribute(console_textcolor, 4); 
                cout << "Invalid vertex!\n";
                SetConsoleTextAttribute(console_textcolor, 0x0F);
                continue;
            }
        break;
    }
}

void Graph::createGraphList() {
    int vert;
    bool dir;
    getGraphData(vert, dir);

    vertices = vert;
    directed = dir;
    matrix = false;

    graph.clear();
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
    SetConsoleTextAttribute(console_textcolor, 2);
    cout << "Your graph was successfully created!\n";
    SetConsoleTextAttribute(console_textcolor, 0x0F);
    listToFile();
}

void Graph::createGraphMatrix() {
    int vert;
    bool dir;
    getGraphData(vert, dir);

    vertices = vert;
    directed = dir;
    matrix = true;

    graph.clear();
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
    SetConsoleTextAttribute(console_textcolor, 2);
    cout << "Your matrix was successfully created!\n";
    SetConsoleTextAttribute(console_textcolor, 0x0F);
    matrixToFile();
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
            cout << graph[i][j] << " ";
        }
        cout << "\n";
    }
}

void Graph::addEdge() {
    cout << "Enter the edge to add (u v) or -1 -1 to exit\n";
    int u, v;
    if (!checkVertices(u, v))
        return;
    if (matrix){
        graph[u][v] = 1; 
        if(!directed) graph[v][u] = 1;
        matrixToFile();
    }
    else {
        graph[u].push_back(v);
        if (!directed) graph[v].push_back(u);
        listToFile();
    }
    SetConsoleTextAttribute(console_textcolor, 2);
    cout << "The edge (" << u << " " << v << ") was added to the graph!\n";
    SetConsoleTextAttribute(console_textcolor, 0x0F);
}

void Graph::removeEdge() {
    cout << "Enter the vertex to remove (u v) or -1 -1 to exit\n";
    int u, v;
    if (!checkVertices(u, v))
        return;
    bool removed = false;
    if (matrix) {
        if(graph[u][v] == 1)
            removed = true;
        graph[u][v] = 0;
        if (!directed) graph[v][u] = 0;
        matrixToFile();
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
                if (*i == u) {
                    graph[v].erase(i);
                    break;
                }
        if (removed)
            listToFile();
    }
    if (!removed) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Edge (" << u << " " << v << ") doesn't exist\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
    }
    else {
        SetConsoleTextAttribute(console_textcolor, 2);
        cout << "The edge (" << u << " " << v << ") was removed from the graph!\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
    }
}

void Graph::isConnected() {
    if (directed) {
        isStronglyConnected();
        return;
    }

    if (vertices == 0) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph has no vertices.\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return;
    }

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
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph has no edges.\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return;
    }

    DFS(start, visited);

    bool allConnected = true;
    for (int i = 0; i < vertices; i++) {
        bool hasEdges = false;
        if (matrix) {
            for (int j = 0; j < vertices; j++)
                if (graph[i][j]) { hasEdges = true; break; }
        }
        else {
            if (!graph[i].empty())
                hasEdges = true;
        }

        if (hasEdges && !visited[i]) {
            allConnected = false;
            break;
        }
    }

    for (int i = 0; i < vertices; i++) {
        if (graph[i].empty()) {
            allConnected = false;
            break;
        }
    }

    if (allConnected) {
        SetConsoleTextAttribute(console_textcolor, 2);
        cout << "Graph is Connected.\n";
    }
    else {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph is not Connected.\n";
    }
    SetConsoleTextAttribute(console_textcolor, 0x0F);
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
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Graph has no edges.\n";
        SetConsoleTextAttribute(console_textcolor, 0x0F);
        return;
    }

    DFS(start, visited);
    for (bool v : visited)
        if (!v) {
            SetConsoleTextAttribute(console_textcolor, 4);
            cout << "Graph is not Connected.\n";
            SetConsoleTextAttribute(console_textcolor, 0x0F);
            return;
        }

    Graph gT = getTranspose();
    for (int i = 0; i < visited.size(); i++)
        visited[i] = false;
    gT.DFS(start, visited);

    for (bool v : visited)
        if (!v) {
            SetConsoleTextAttribute(console_textcolor, 4);
            cout << "Graph is not Connected.\n";
            SetConsoleTextAttribute(console_textcolor, 0x0F);
            return;
        }
    SetConsoleTextAttribute(console_textcolor, 2);
    cout << "Graph is Connected.\n";
    SetConsoleTextAttribute(console_textcolor, 0x0F);
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
    if (dist[end] == -1) {
        SetConsoleTextAttribute(console_textcolor, 4);
        cout << "Can't find a way between vertices\n";
    }
    else {
        SetConsoleTextAttribute(console_textcolor, 2);
        cout << "Distance between the vertices is " << dist[end] << endl;
    }
    SetConsoleTextAttribute(console_textcolor, 0x0F);
}