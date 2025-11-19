#include <iostream>
#include "graph.h"

/**
 * @brief Entry point of the graph application.
 *
 * The program tries to load a graph from file. If the file does not exist,
 * it offers the user to create a new graph (list or matrix).
 * Afterwards, the user can repeatedly:
 *  - print the graph,
 *  - add or remove edges,
 *  - check connectivity,
 *  - compute distance between two vertices,
 *  - or recreate the graph.
 *
 * @return Exit status code (0 for normal termination, 1 when exiting via menu).
 * @throws None.
 * @example
 * // Typical execution flow:
 * // 1. User starts program.
 * // 2. Chooses "Create new Graph as an Adjacency List".
 * // 3. Adds edges and checks connectivity.
 */

int main() {
	Graph grph;
	char choice;
	if (!grph.loadFromFile()) {
	tryAgain:
		cout << "\n------------------GRAPHS------------------\n";
		cout << "1.Create new Graph as an Adjacency List\n";
		cout << "2.Create new Graph as an Adjacency Matrix\n";
		cout << "3.exit\n";
		cout << "------------------------------------------\n";
		cout << "Enter Your Choice :: ";
		cin >> choice;
		switch (choice)
		{
		case '1':
			cin.ignore();
			grph.createGraphList();
			break;
		case '2':
			cin.ignore();
			grph.createGraphMatrix();
			break;
		case '3':
			return 1;
		default:
			cout << "Invalid Input...\n";
			cin.ignore();
			goto tryAgain;
		}
	}
	while (true) {
		cout << "\n----------------------------GRAPHS----------------------------\n";
		cout << "1.Create new Graph as an Adjacency List\n";
		cout << "2.Create new Graph as an Adjacency Matrix\n";
		cout << "3.Print your Graph\n";
		cout << "4.Add new edge to your Graph\n";
		cout << "5.Remove an edge from your Graph\n";
		cout << "6.Check if your graph is Connected\n";
		cout << "7.Check the distance between two vertices in your Graph\n";
		cout << "8.exit\n";
		cout << "--------------------------------------------------------------\n\n";
		cout << "Enter Your Choice :: ";
		cin >> choice;

		switch (choice)
		{
		case '1':
			cin.ignore();
			grph.createGraphList();
			break;
		case '2':
			cin.ignore();
			grph.createGraphMatrix();
			break;
		case '3':
			if (grph.getMatrix())
				grph.printGraphMatrix();
			else
				grph.printGraphList();
			break;
		case '4':
			cin.ignore();
			grph.addEdge();
			break;
		case '5':
			cin.ignore();
			grph.removeEdge();
			break;
		case '6':
			grph.isConnected();
			break;
		case '7':
			grph.distanceBetween();
			break;
		case '8':
			return 1;
		default:
			cout << "Invalid input...\n";
			cin.ignore();
			break;
		}
	}

}