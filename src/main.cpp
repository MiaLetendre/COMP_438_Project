#include "vineGen.h"
#include "vine.h"
#include "node.h"
#include "Turt.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <iostream>
#include <vector>
#include <unordered_map>


namespace py = pybind11;

using namespace std;

int iterations = 5;
string start = "F";
//each ine=dex of the rule string matches a replace string
//rules I have tried so far: 
// F -> F[+F]F[-F]F
// F -> F[+&F<]F[-^F]F>, 
unordered_map<string, string> rules = {{"F", "F[+&F<]F[-^F]F>"}};

int main() {
	/*
	cout << "main is working" << endl;
	VineGen gen(iterations, start, rules);
	string LString = gen.getLString(iterations, start, rules);
	cout << "LString: " << LString << endl;
	vector<vector<Eigen::Vector3f>> totalBranches = gen.generate_vine(LString);
	totalBranches = gen.splineIt();
	cout << "Total Branches: " << totalBranches.size() << endl;
	for (int i = 0; i < totalBranches.size(); i++) {
		for (int j = 0; j < totalBranches[i].size(); j++) {
			cout << totalBranches[i][j] << endl;
		}
	}
	*/
	return 0;
}

vector<vector<Eigen::Vector3f>> generate_vine_main(int iterations, string start, unordered_map<string, string> rules) {
	//starting rules will be above, willbe allowed to edit it later on

	VineGen gen(iterations, start, rules);
	string LString = gen.getLString(iterations, start, rules);
	vector<vector<Eigen::Vector3f>> totalBranches = gen.generate_vine(LString);
	totalBranches = gen.splineIt();
	return totalBranches;
}




PYBIND11_MODULE(vineGen_cpp, m) {
    m.doc() = "C++ backend for vine generation";

  
	m.def("generate_vine", &generate_vine_main, "Takes rules, axioms, etc, turns it into string, turns that int coords",
		py::arg("iterations"),
		py::arg("start"),
		py::arg("rules"));
}