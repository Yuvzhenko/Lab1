#include <iostream>
#include "graph.h"

int main() {
	int v;
	cout << "Enter the amount of vertices :: ";
	cin >> v;
	cin.ignore();
	cout << "Is it directed (yes/no) :: ";
	string d;
	bool dir;
	getline(cin, d);
	if (trim(d) == "yes")
		dir = true;
	else
		dir = false;

	Graph grph(v, dir);

	grph.createGraphMatrix();
	grph.distanceBetween();
}