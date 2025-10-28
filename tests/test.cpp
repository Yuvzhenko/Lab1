#include "pch.h"
#include "../graph.h"

using namespace std;

class TestableGraph : public Graph {
public:
    using Graph::DFS;
    using Graph::setVert;
    using Graph::setDir;
    using Graph::getMatrix;
    using Graph::isConnected;
    using Graph::addEdge;
    using Graph::removeEdge;
    using Graph::distanceBetween;
    using Graph::isStronglyConnected;
    using Graph::getGraphData;
    using Graph::listToFile;
    using Graph::matrixToFile;

    vector<vector<int>>& data() { return graph; }
    void setMatrix(bool m) { matrix = m; }
    void setData(const vector<vector<int>>& g) { graph = g; }

    bool checkVertices(int& u, int& v) {
        cin >> u >> v;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(32767, '\n');
            cout << "Invalid vertex!\n";
            return false;
        }
        if (u == -1 && v == -1)
            return false;
        if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
            cout << "Invalid vertex!\n";
            return false;
        }
        return true;
    }

    TestableGraph getTranspose() {
        TestableGraph gT;
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
};

TEST(GraphTest, AddEdge_Matrix_Undirected_AddsBothDirections) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,0,0}, {0,0,0}, {0,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = std::cin.rdbuf(input.rdbuf());
    g.addEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(g.data()[0][1], 1);
    EXPECT_EQ(g.data()[1][0], 1);
}

TEST(GraphTest, AddEdge_Matrix_Directed_AddsOneDirection) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(true);

    g.setData({ {0,0,0}, {0,0,0}, {0,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.addEdge();

    EXPECT_EQ(g.data()[0][1], 1);
    EXPECT_EQ(g.data()[1][0], 0);
}

TEST(GraphTest, AddEdge_Matrix_DuplicateEdge_DoesNotCorruptState) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);
    g.setData({ {0,0,0}, {0,0,0}, {0,0,0} });

    // Adding one edge twice
    {
        stringstream dummy;
        streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
        stringstream input("0 1\n");
        streambuf* orig_cin = cin.rdbuf(input.rdbuf());
        g.addEdge();
        cin.rdbuf(orig_cin);
        cout.rdbuf(orig_cout);
    }
    {
        stringstream dummy;
        streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
        stringstream input("0 1\n");
        streambuf* orig_cin = cin.rdbuf(input.rdbuf());
        g.addEdge();
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
    }

    EXPECT_EQ(g.data()[0][1], 1);
    EXPECT_EQ(g.data()[1][0], 1);
    EXPECT_EQ(g.data()[0][2], 0);
    EXPECT_EQ(g.data()[1][2], 0);
    EXPECT_EQ(g.data()[2][0], 0);
    EXPECT_EQ(g.data()[2][1], 0);
}

TEST(GraphTest, AddEdge_Matrix_Loop_AddsSelfLoop) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,0,0}, {0,0,0}, {0,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("1 1\n");
    streambuf* orig_cin = std::cin.rdbuf(input.rdbuf());
    g.addEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(g.data()[1][1], 1);
}

TEST(GraphTest, RemoveEdge_Matrix_Undirected_RemovesBothDirections) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,1,0}, 
                {1,0,0}, 
                {0,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(g.data()[0][1], 0);
    EXPECT_EQ(g.data()[1][0], 0);
}

TEST(GraphTest, RemoveEdge_Matrix_Directed_RemovesOneDirection) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(true);

    g.setData({ {0,1,0},
                {1,0,0},
                {0,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(g.data()[0][1], 0);
    EXPECT_EQ(g.data()[1][0], 1);
}

TEST(GraphTest, RemoveEdge_Matrix_SelfLoop_RemovesOnlyLoop) {
    TestableGraph g;
    g.setVert(2);
    g.setDir(false);
    g.setMatrix(true);
    g.setData({ {1,1}, {1,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 0\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(g.data()[0][0], 0);
    EXPECT_EQ(g.data()[0][1], 1);
    EXPECT_EQ(g.data()[1][0], 1);
}

TEST(GraphTest, RemoveEdge_Matrix_NonExistentEdge_PrintsInvalidMessageNoChange) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,0,0},
                {0,0,0},
                {0,0,0} });

    auto original = g.data();

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    g.removeEdge();

    cout.rdbuf(orig_cout);
    cin.rdbuf(orig_cin);

    EXPECT_EQ(g.data(), original);
    EXPECT_NE(dummy.str().find("Edge (0 1) doesn't exist"), string::npos);
}

TEST(GraphTest, AddEdge_List_Undirected_AddsBothDirections) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {}, {}, {} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.addEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) != g.data()[1].end());
}

TEST(GraphTest, AddEdge_List_Directed_AddsOneDirection) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(false);

    g.setData({ {}, {}, {} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.addEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) == g.data()[1].end());
}

TEST(GraphTest, AddEdge_List_DuplicateEdge_DoesNotCorruptState) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);
    g.setData({ {}, {}, {} });

    // Adding one edge twice
    {
        stringstream dummy;
        streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
        stringstream input("0 1\n");
        streambuf* orig_cin = cin.rdbuf(input.rdbuf());
        g.addEdge();
        cin.rdbuf(orig_cin);
        cout.rdbuf(orig_cout);
    }
    {
        stringstream dummy;
        streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
        stringstream input("0 1\n");
        streambuf* orig_cin = cin.rdbuf(input.rdbuf());
        g.addEdge();
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
    }

    // checking if the edge is exsist and the way didn't corrupted
    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) != g.data()[1].end());
    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 2) == g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 2) == g.data()[1].end());
    EXPECT_TRUE(g.data()[2].empty());
}

TEST(GraphTest, AddEdge_List_Loop_AddsSelfLoop) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {}, {}, {} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("1 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.addEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 1) != g.data()[1].end());
}

TEST(GraphTest, RemoveEdge_List_Undirected_RemovesBothDirections) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {1, 2}, {0}, {0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) == g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) == g.data()[1].end());
    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 2) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[2].begin(), g.data()[2].end(), 0) != g.data()[2].end());
}

TEST(GraphTest, RemoveEdge_List_Directed_RemovesOneDirection) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(false);

    g.setData({ {1, 2}, {0}, {0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) == g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) != g.data()[1].end());
    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 2) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[2].begin(), g.data()[2].end(), 0) != g.data()[2].end());
}

TEST(GraphTest, RemoveEdge_List_SelfLoop_RemovesOnlyLoop) {
    TestableGraph g;
    g.setVert(2);
    g.setDir(false);
    g.setMatrix(false);
    g.setData({ {0,1}, {0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 0\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    g.removeEdge();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 0) == g.data()[0].end());
    EXPECT_TRUE(find(g.data()[0].begin(), g.data()[0].end(), 1) != g.data()[0].end());
    EXPECT_TRUE(find(g.data()[1].begin(), g.data()[1].end(), 0) != g.data()[1].end());
}

TEST(GraphTest, RemoveEdge_List_NonExistentEdge_PrintsInvalidMessageNoChange) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {}, {}, {} });
    auto original = g.data();

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    g.removeEdge();

    cout.rdbuf(orig_cout);
    cin.rdbuf(orig_cin);

    EXPECT_EQ(g.data(), original);
    EXPECT_NE(dummy.str().find("Edge (0 1) doesn't exist"), string::npos);
}

TEST(GraphTest, CheckVertices_InputVariousValidity_HandlesProperly) {
    TestableGraph g;
    g.setVert(3);

    int u = -1, v = -1;

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("0 1\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    EXPECT_TRUE(g.checkVertices(u, v));
    EXPECT_EQ(u, 0);
    EXPECT_EQ(v, 1);

    input.clear();
    input.str("5 2\n");
    input.seekg(0);
    cin.rdbuf(input.rdbuf());

    EXPECT_FALSE(g.checkVertices(u, v));
    string output = dummy.str();
    EXPECT_NE(output.find("Invalid vertex!"), string::npos);

    input.clear();
    input.str("- 1 -1\n");
    input.seekg(0);
    cin.rdbuf(input.rdbuf());

    EXPECT_FALSE(g.checkVertices(u, v));

    dummy.str("");
    input.clear();
    input.str("hkvb bbf\n");
    input.seekg(0);
    cin.rdbuf(input.rdbuf());

    EXPECT_FALSE(g.checkVertices(u, v));
    output = dummy.str();
    EXPECT_NE(output.find("Invalid vertex!"), string::npos);

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);
}

TEST(GraphTest, DFS_List_ValidGraph_TraversesAllReachableVertices) {
    TestableGraph g;
    g.setVert(5);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({{1}, {0, 2}, {1, 3}, {2}, {} });

    vector<bool> visited(5, false);
    g.DFS(0, visited);  

    EXPECT_TRUE(visited[0]);
    EXPECT_TRUE(visited[1]);
    EXPECT_TRUE(visited[2]);
    EXPECT_TRUE(visited[3]);
    EXPECT_FALSE(visited[4]); 
}

TEST(GraphTest, DFS_Matrix_ValidGraph_TraversesAllReachableVertices) {
    TestableGraph g;
    g.setVert(5);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,1,0,0,0},
                {1,0,1,0,0},
                {0,1,0,1,0},
                {0,0,1,0,0},
                {0,0,0,0,0} });

    vector<bool> visited(5, false);
    g.DFS(0, visited);

    EXPECT_TRUE(visited[0]);
    EXPECT_TRUE(visited[1]);
    EXPECT_TRUE(visited[2]);
    EXPECT_TRUE(visited[3]);
    EXPECT_FALSE(visited[4]);
}

TEST(GraphTest, IsConnected_List_Undirected_ConnectedAndDisconnectedOutputsCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {1}, {0, 2}, {1} });

    stringstream dummy;

    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
    g.isStronglyConnected();
    cout.rdbuf(orig_cout);

    string output = dummy.str();
    EXPECT_NE(output.find("Graph is Connected"), string::npos);

    g.setData({ {1}, {0}, {} });

    orig_cout = cout.rdbuf(dummy.rdbuf());
    g.isConnected();
    cout.rdbuf(orig_cout);

    output = dummy.str();
    EXPECT_NE(output.find("Graph is not Connected"), string::npos);
}

TEST(GraphTest, IsConnected_List_Directed_ConnectedAndDisconnectedOutputsCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(false);

    g.setData({ {1}, {0, 2}, {0} });

    stringstream dummy;
    
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());
    g.isStronglyConnected();
    cout.rdbuf(orig_cout);
    
    string output = dummy.str();
    EXPECT_NE(output.find("Graph is Connected"), string::npos);

    g.setData({ {1}, {2}, {} });
    
    orig_cout = cout.rdbuf(dummy.rdbuf());
    g.isStronglyConnected();
    cout.rdbuf(orig_cout);
    
    output = dummy.str();
    EXPECT_NE(output.find("Graph is not Connected"), string::npos);
}

TEST(GraphTest, IsConnected_List_NoVertices_PrintsNoVerticesMessage) {
    TestableGraph g;
    g.setVert(0);
    g.setDir(false);
    g.setMatrix(false);

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("Graph has no vertices"), string::npos);
}

TEST(GraphTest, IsConnected_Matrix_Undirected_ConnectedDisconnectedAndNoEdgesOutputs) {
    TestableGraph g;
    g.setVert(4);
    g.setDir(false);
    g.setMatrix(true);

    // 0𥹒�3
    g.setData({
        {0,1,0,0},
        {1,0,1,0},
        {0,1,0,1},
        {0,0,1,0}
        });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("Graph is Connected"), string::npos);

    // 0�1 2�3
    g.setData({
        {0,1,0,0},
        {1,0,0,0},
        {0,0,0,1},
        {0,0,1,0}
        });

    orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    output = dummy.str();
    EXPECT_NE(output.find("Graph is not Connected"), string::npos);

    // without edges
    g.setData({
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
        });

    orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    output = dummy.str();
    EXPECT_NE(output.find("Graph has no edges"), string::npos);
}

TEST(GraphTest, IsConnected_Matrix_Directed_ConnectedDisconnectedAndNoEdgesOutputs) {
    TestableGraph g;
    g.setVert(4);
    g.setDir(true);
    g.setMatrix(true);

    // 0𥹒�3
    g.setData({
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
        {1,0,0,0}
        });

    stringstream dummy1;
    streambuf* orig_cout = cout.rdbuf(dummy1.rdbuf());

    g.isStronglyConnected();

    cout.rdbuf(orig_cout);
    string output = dummy1.str();
    EXPECT_NE(output.find("Graph is Connected"), string::npos);

    // 0�1 2�3
    g.setData({
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1},
        {0,0,0,0}
        });

    stringstream dummy2;
    orig_cout = cout.rdbuf(dummy2.rdbuf());

    g.isStronglyConnected();

    cout.rdbuf(orig_cout);
    output = dummy2.str();
    EXPECT_NE(output.find("Graph is not Connected"), string::npos);

    // without edges
    g.setData({
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
        });

    stringstream dummy3;
    orig_cout = cout.rdbuf(dummy3.rdbuf());

    g.isStronglyConnected();

    cout.rdbuf(orig_cout);
    output = dummy3.str();
    EXPECT_NE(output.find("Graph has no edges"), string::npos);
}

TEST(GraphTest, IsConnected_Matrix_NoVertices_PrintsNoEdgesMessage) {
    TestableGraph g;
    g.setVert(0);
    g.setDir(false);
    g.setMatrix(true);

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("Graph has no vertices"), string::npos);
}

TEST(GraphTest, IsConnected_SingleVertex_NoEdges_PrintsNoEdgesMessage) {
    TestableGraph g;
    g.setVert(1);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("Graph has no edges"), string::npos);
}

TEST(GraphTest, DistanceBetween_List_ValidAndInvalidPaths_OutputCorrectMessage) {
    TestableGraph g;
    g.setVert(5);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {1}, {0, 2}, {1, 3}, {2}, {} });

    istringstream input("0 3\n");
    ostringstream dummy;
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.distanceBetween();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    string output = dummy.str();
    EXPECT_NE(output.find("Distance between the vertices is 3"), string::npos);

    /////////////////////////
    input.str("0 4\n");
    orig_cin = cin.rdbuf(input.rdbuf());
    orig_cout = cout.rdbuf(dummy.rdbuf());

    g.distanceBetween();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    output = dummy.str();
    EXPECT_NE(output.find("Can't find a way between vertices"), string::npos);
}

TEST(GraphTest, DistanceBetween_Matrix_ValidAndInvalidPaths_OutputCorrectMessage) {
    TestableGraph g;
    g.setVert(5);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,1,0,0,0},
                {1,0,1,0,0},
                {0,1,0,1,0},
                {0,0,1,0,0},
                {0,0,0,0,0} });

    istringstream input("0 3\n");
    ostringstream dummy;
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.distanceBetween();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    string output = dummy.str();
    EXPECT_NE(output.find("Distance between the vertices is 3"), string::npos);

    /////////////////////////
    input.str("0 4\n");
    orig_cin = cin.rdbuf(input.rdbuf());
    orig_cout = cout.rdbuf(dummy.rdbuf());

    g.distanceBetween();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    output = dummy.str();
    EXPECT_NE(output.find("Can't find a way between vertices"), string::npos);
}

TEST(GraphTest, GetGraphData_InputVariousValidity_AssignsCorrectValues) {
    int v;
    bool dir;
    TestableGraph g;

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    stringstream input("5\n yes\n");
    streambuf* orig_cin = cin.rdbuf(input.rdbuf());

    g.getGraphData(v, dir);

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);
    
    EXPECT_EQ(v, 5);
    EXPECT_EQ(dir, true);

    orig_cout = cout.rdbuf(dummy.rdbuf());

    input.str("0\n 4\n gg\n no\n");
    orig_cin = cin.rdbuf(input.rdbuf());

    g.getGraphData(v, dir);

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    EXPECT_EQ(v, 4);
    EXPECT_EQ(dir, false); 
}

TEST(GraphTest, GetTranspose_List_ReversesEdgesCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(false);

    g.setData({ {1}, {2}, {0} });

    TestableGraph gT = g.getTranspose();

    EXPECT_TRUE(find(gT.data()[0].begin(), gT.data()[0].end(), 2) != gT.data()[0].end());
    EXPECT_TRUE(find(gT.data()[2].begin(), gT.data()[2].end(), 1) != gT.data()[2].end());
    EXPECT_TRUE(find(gT.data()[1].begin(), gT.data()[1].end(), 0) != gT.data()[1].end());
}

TEST(GraphTest, GetTranspose_Matrix_ReversesEdgesCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(true);
    g.setMatrix(true);

    g.setData({ {0,1,0},
                {0,0,1},
                {1,0,0} });

    TestableGraph gT = g.getTranspose();

    EXPECT_EQ(gT.data()[0][2], 1);
    EXPECT_EQ(gT.data()[2][1], 1);
    EXPECT_EQ(gT.data()[1][0], 1);
}

TEST(GraphTest, PrintGraph_List_OutputsGraphStructureCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {1, 2}, {0,2}, {0, 1} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.printGraphList();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("0: 1 2"), string::npos);
    EXPECT_NE(output.find("1: 0 2"), string::npos);
    EXPECT_NE(output.find("2: 0 1"), string::npos);
}

TEST(GraphTest, PrintGraph_Matrix_OutputsGraphStructureCorrectly) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,1,0},
                {0,0,1},
                {1,0,0} });

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.printGraphList();

    cout.rdbuf(orig_cout);
    string output = dummy.str();
    EXPECT_NE(output.find("0 1 0"), string::npos);
    EXPECT_NE(output.find("0 0 1"), string::npos);
    EXPECT_NE(output.find("1 0 0"), string::npos);
}

TEST(GraphTest, ListToFile_ListGraph_WritesCorrectFormat) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(false);

    g.setData({ {1, 2}, {0}, {0} });

    g.listToFile();

    ifstream in("graph.txt");
    ASSERT_TRUE(in.is_open());

    string line;
    vector<string> lines;

    while (getline(in, line))
        lines.push_back(line);
    in.close();

    // validate the correctness of the file contents
    ASSERT_GE(lines.size(), 4);

    EXPECT_EQ(lines[0], "3 0 0");

    EXPECT_EQ(lines[1], "0: 1 2 ");
    EXPECT_EQ(lines[2], "1: 0 ");
    EXPECT_EQ(lines[3], "2: 0 ");

    remove("graph.txt");
}

TEST(GraphTest, MatrixToFile_MatrixGraph_WritesCorrectFormat) {
    TestableGraph g;
    g.setVert(3);
    g.setDir(false);
    g.setMatrix(true);

    g.setData({ {0,1,0},
                {1,0,1},
                {0,1,0} });

    g.matrixToFile();

    ifstream in("graph.txt");
    ASSERT_TRUE(in.is_open());

    string line;
    vector<string> lines;

    while (getline(in, line))
        lines.push_back(line);
    in.close();
    // validate the correctness of the file contents
    ASSERT_GE(lines.size(), 4);

    EXPECT_EQ(lines[0], "3 0 1");

    EXPECT_EQ(lines[1], "0 1 0 ");
    EXPECT_EQ(lines[2], "1 0 1 ");
    EXPECT_EQ(lines[3], "0 1 0 ");

    remove("graph.txt");
}

TEST(GraphTest, Integration_List_Undirected_AddEdgesThenConnectedGraphsOutputsConnected) {
    TestableGraph g;
    g.setVert(4);
    g.setDir(false);
    g.setMatrix(false);
    g.setData({ {}, {}, {}, {} });

    auto addEdgeIO = [&](int u, int v) {
        stringstream dummy;
        streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

        stringstream input; input << u << " " << v << "\n";
        streambuf* orig_cin = cin.rdbuf(input.rdbuf());

        g.addEdge();

        cin.rdbuf(orig_cin);
        cout.rdbuf(orig_cout);
        };

    addEdgeIO(0, 1);
    addEdgeIO(1, 2);
    addEdgeIO(2, 3);

    stringstream cap;
    streambuf* orig_cout = cout.rdbuf(cap.rdbuf());
    g.isConnected();
    cout.rdbuf(orig_cout);

    string out = cap.str();
    EXPECT_NE(out.find("Graph is Connected"), string::npos);
}

TEST(GraphTest, Performance_List_LargeSparseGraph_IsConnectedCompletesQuickly) {
    const int N = 2000; 

    TestableGraph g;
    g.setVert(N);
    g.setDir(false);
    g.setMatrix(false);

    g.setData(vector<vector<int>>(N));

    for (int i = 0; i < N - 1; ++i) {
        g.data()[i].push_back(i + 1);
        g.data()[i + 1].push_back(i);
    }

    stringstream cap;
    streambuf* orig_cout = cout.rdbuf(cap.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);
    string out = cap.str();
    EXPECT_NE(out.find("Graph is Connected"), string::npos);
}

TEST(GraphTest, Performance_Matrix_LargeSparseGraph_IsConnectedCompletesQuickly) {
    const int N = 2000;

    TestableGraph g;
    g.setVert(N);
    g.setDir(false);
    g.setMatrix(true);

    g.setData(vector<vector<int>>(N, vector<int>(N, 0)));

    for (int i = 0; i < N - 1; ++i)
        g.data()[i][i + 1] = g.data()[i + 1][i] = 1;

    stringstream dummy;
    streambuf* orig_cout = cout.rdbuf(dummy.rdbuf());

    g.isConnected();

    cout.rdbuf(orig_cout);

    EXPECT_NE(dummy.str().find("Graph is Connected"), string::npos);
}
