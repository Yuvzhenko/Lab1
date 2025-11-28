#pragma once
#include <iostream>

#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>

#include <fstream>

using namespace std;

/**
 * @brief Trims leading and trailing whitespace characters from a string.
 *
 * Removes spaces, newlines, carriage returns and tab characters from
 * the beginning and the end of the given string.
 *
 * @param source Input string that may contain leading/trailing whitespace.
 * @return A copy of @p source without leading and trailing whitespace.
 * @throws None. This function does not throw exceptions explicitly.
 * @example
 * std::string original = "  42 \n";
 * std::string cleaned  = trim(original); // "42"
 */
string trim(const string& source);

/**
 * @brief Represents an unweighted graph that can be stored as an adjacency
 *        list or adjacency matrix.
 *
 * The Graph class provides operations to create a graph from user input
 * or from a file, print it, add and remove edges, check connectivity,
 * and compute the distance (in edges) between two vertices.
 *
 * The graph can be directed or undirected and stored either as an
 * adjacency list or adjacency matrix, depending on user choice.
 *
 * @example
 * Graph g;
 * if (!g.loadFromFile()) {
 *     // If file does not exist – create a new graph as adjacency list
 *     g.createGraphList();
 * }
 * // Print and check connectivity
 * if (g.getMatrix())
 *     g.printGraphMatrix();
 * else
 *     g.printGraphList();
 * g.isConnected();
 */
class Graph {
protected:
	/// Underlying container for adjacency list / adjacency matrix.
	vector<vector<int>> graph;
	/// File stream used for saving and loading graphs.
	fstream file;
	/// Indicates whether the graph is directed (true) or undirected (false).
	bool directed;
	/// Indicates whether representation is adjacency matrix (true) or list (false).
	bool matrix;
	/// Number of vertices in the graph.
	int vertices;
public:
	/**
	 * @brief Sets the number of vertices in the graph.
	 *
	 * Does not allocate or modify the adjacency structure by itself,
	 * only stores the value of @p v.
	 *
	 * @param v New number of vertices (should be >= 0).
	 * @return void
	 * @throws None.
	 */
	void setVert(int v) { vertices = v; }

	/**
	 * @brief Sets the directed/undirected mode of the graph.
	 *
	 * @param dir If true, the graph is treated as directed; otherwise undirected.
	 * @return void
	 * @throws None.
	 */
	void setDir(bool dir) { directed = dir; }

	/**
	 * @brief Returns whether the graph is stored as an adjacency matrix.
	 *
	 * @return true if the graph is stored as a matrix, false if as a list.
	 * @throws None.
	 */
	bool getMatrix() { return matrix; }

	/**
	 * @brief Interactively creates a graph using adjacency list representation.
	 *
	 * Asks the user for the number of vertices and whether the graph is directed,
	 * then reads edges from standard input until the pair "-1 -1" is entered.
	 * The created graph is saved to a file.
	 *
	 * @return void
	 * @throws None. All invalid user input is handled via messages and re-prompts.
	 * @example
	 * Graph g;
	 * g.createGraphList();
	 */
	void createGraphList();		

	/**
	 * @brief Interactively creates a graph using adjacency matrix representation.
	 *
	 * Asks the user for number of vertices and directed/undirected mode,
	 * then reads edges from standard input until "-1 -1" is entered.
	 * The created matrix is saved to a file.
	 *
	 * @return void
	 * @throws None.
	 */
	void createGraphMatrix();

	/**
	* @brief Prints the graph in adjacency list form to standard output.
	*
	* Each line has the format "i: v1 v2 ...".
	*
	* @return void
	* @throws None.
	*/
	void printGraphList();

	/**
	 * @brief Prints the graph in adjacency matrix form to standard output.
	 *
	 * Each line represents a row of the matrix with 0/1 entries.
	 *
	 * @return void
	 * @throws None.
	 */
	void printGraphMatrix();

	/**
	 * @brief Adds an edge to the current graph.
	 *
	 * Reads a pair of vertices (u v) from standard input and, if the pair
	 * is valid, inserts the edge into the adjacency list or matrix.
	 * For undirected graphs, both directions are added.
	 * The updated graph is written to file.
	 *
	 * @return void
	 * @throws None.
	 * @example
	 * // After creating a graph, call:
	 * g.addEdge(); // user enters: 0 1
	 */
	void addEdge();

	/**
	 * @brief Removes an edge from the current graph.
	 *
	 * Reads a pair of vertices (u v) from standard input and, if the edge
	 * exists, removes it. For undirected graphs, both directions are removed.
	 * The updated graph is written to file.
	 *
	 * @return void
	 * @throws None.
	 */
	void removeEdge();

	/**
	 * @brief Checks whether the graph is connected (undirected) or strongly
	 *        connected (directed).
	 *
	 * For undirected graphs, performs a DFS from a vertex that has edges
	 * and verifies that all vertices with edges are reachable.
	 * For directed graphs, it delegates to isStronglyConnected().
	 *
	 * Messages about connectivity are printed to standard output.
	 *
	 * @return void
	 * @throws None.
	 */
	void isConnected();

	/**
	 * @brief Computes the shortest distance (in edges) between two vertices.
	 *
	 * Uses BFS over the current representation (list or matrix) to find
	 * the minimum number of edges between given vertices. If no path exists,
	 * prints an error message.
	 *
	 * @return void
	 * @throws None.
	 * @example
	 * g.distanceBetween(); // user enters: 0 3
	 */
	void distanceBetween();

	/**
	 * @brief Loads a graph from the file "graph.txt".
	 *
	 * Reads number of vertices, directed flag and representation type,
	 * then reconstructs adjacency list or matrix from the file.
	 *
	 * @return true if the graph was successfully loaded, false otherwise.
	 * @throws None. File opening failures are reported via return value.
	 */
	bool loadFromFile();

protected:
	/**
	 * @brief Interactively obtains the number of vertices and direction flag.
	 *
	 * Prompts the user until valid values are entered.
	 *
	 * @param v Reference where the number of vertices will be stored.
	 * @param dir Reference where the directed flag will be stored.
	 * @return void
	 * @throws None.
	 */
	void getGraphData(int& v, bool& dir);

	/**
	 * @brief Depth-first search from a given vertex.
	 *
	 * This helper recursively marks all reachable vertices from @p v
	 * in the @p visited array.
	 *
	 * @param v Starting vertex index.
	 * @param visited Reference to boolean vector storing visited flags.
	 * @return void
	 * @throws None.
	 */
	void DFS(int v, vector<bool>& visited);

	/**
	 * @brief Checks if the directed graph is strongly connected.
	 *
	 * Uses a DFS in the original graph and in its transpose to verify
	 * that every vertex is reachable from every other vertex.
	 *
	 * @return void
	 * @throws None.
	 */
	void isStronglyConnected();

	/**
	 * @brief Saves the current graph in adjacency matrix form to "graph.txt".
	 *
	 * The first line contains number of vertices, directed flag and matrix flag.
	 * Next lines contain rows of the adjacency matrix.
	 *
	 * @return void
	 * @throws None.
	 */
	const void matrixToFile();

	/**
	 * @brief Saves the current graph in adjacency list form to "graph.txt".
	 *
	 * The first line contains number of vertices, directed flag and matrix flag.
	 * Each next line has the form "i: v1 v2 ...".
	 *
	 * @return void
	 * @throws None.
	 */
	const void listToFile();

	/**
	 * @brief Validates a pair of vertices read from standard input.
	 *
	 * Reads a pair (u v) from stdin and checks that both vertices are
	 * within the valid range [0, vertices). Special pair "-1 -1" is treated
	 * as a signal to cancel the operation.
	 *
	 * @param u Reference where the first vertex index will be stored.
	 * @param v Reference where the second vertex index will be stored.
	 * @return true if a valid pair of vertices was entered, false if the user
	 *         entered "-1 -1" or if reading failed irrecoverably.
	 * @throws None.
	 */
	bool checkVertices(int &u, int &v);

	/**
	 * @brief Builds and returns the transpose of the current graph.
	 *
	 * For adjacency matrix representation, this function flips edges along
	 * the main diagonal. For adjacency list representation, it reverses
	 * all directions of edges (u -> v becomes v -> u).
	 *
	 * @return A Graph object representing the transposed graph.
	 * @throws None.
	 */
	Graph getTranspose();
};