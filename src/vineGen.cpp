#include "vineGen.h"
#include "node.h"
#include "Turt.h"

#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <iostream>
#include <unordered_map>
#include <utility>

using namespace std;

VineGen::VineGen(int iterations, string start, unordered_map<string, string> rules) {
	this->iterations = iterations;
	this->start = start;
	this->rules = rules;
}

string VineGen::getLString(int iterations, string start, unordered_map<string, string> rules) {
	//Language : F(forward), +(turn right), -(turn left), [ (bracnh/push), ] (return/pop)
	//Deafult Axiom: F
	//Default Rule(s) : F -> F[+F]F[-F]F


	//NEEDS SINGLE CHAR RULE -> ANY # OF CHARS REPLACEMENT

	//curently just iterates 5 ties on a 2d plane
	//replaces all instances of a rule to replace with what its supposed to
	string LString = start;
	for (int i = 0; i < iterations; i++) {
		string nextString = "";

		for (char c : LString) {
			auto it = rules.find(string(1, c));
			if (it != rules.end()) {
				nextString += it->second;
			}
			else {
				nextString += c;
			}
		}
		LString = nextString;
	}
	return LString;
}

//outer vector is all bracnhes, inner is the 3d coors with the depth
//this the best for the pybind as all blender needs is positions and depth
vector<pair<vector<Eigen::Vector3f>, int>> VineGen::generateVine(string LString, float angle) {
	Node* temp = new Node();
	Turt turtle(temp, angle);

	//turns the string, into the nodes that make the vine
	turtle.processString(LString);
	cout << "Vine Generation activated!" << endl;
	vector<pair<vector<Eigen::Vector3f>, int>> totalBranches;
	if (turtle.getRoot() == nullptr) {
		//shoudlnt get here anymore but saftey
		cout << "Error: Root node is null!" << endl;
		return totalBranches;
	}
	
	turtle.getRoot()->harvest(totalBranches, vector<Eigen::Vector3f>(), 0);
	verts = totalBranches;
	//mem leak fix
	delete temp;
	return totalBranches;
}

vector<pair<vector<Eigen::Vector3f>, int>> VineGen::splineIt() {
	/*Spline Options:
	* Catmull-Rom C1, interpolating, but need tangents, no control at enpoint derive
	* cubic hermite: C1, interpolating must specify derivatives at each endpoint
	* Bezier: C1, NOT interpolating, but outliers are problems
	* Akima: non smoothing
	* monotone cubic: C1, non smoothing
	* 
	* WINNER: catmull
	* 
	*/
	//omg this is hell math 
	//variables needes
	//Pi-1, Pi, Pi+1, Pi+2 anchor, start, end front
	//t to represent increment, i think .2 or .1 shoudl work, depends of performance
	//P(s) = T * M * G
	// T = [t^3, t^2, t, 1] spline coefs
	// M =  [-s,	2-s,	s-2,	s] CR basis 
	//		[2s,	s-3,	3-2s,	-s]
	//  	[-s,	0,		s,		0]
	//		[0,		1,		0,		0]
	// s is typically 1/2 ig
	// G = [Pi-1, Pi, Pi+1, Pi+2] control vectors

	vector<pair<vector<Eigen::Vector3f>, int>> points;
	//
	vector<Eigen::Vector3f> currentBranch;

	if (verts.size() == 0) {
		return points;
	}
	for (int i = 0; i < (int)verts.size(); i++) {
		vector<Eigen::Vector3f> branchVerts = verts[i].first;
		int branchDepth = verts[i].second;
		if (branchVerts.size() < 2) {
			continue;
		}
		for (int j = 0; j < (int)branchVerts.size() - 1; j++) {
			if (j == 0) { //need a ghost point if no prev index
				p0 = branchVerts[j] - (branchVerts[j + 1] - branchVerts[j]);
			}
			else {
				p0 = branchVerts[j - 1];
			}
			p1 = branchVerts[j];
			p2 = branchVerts[j + 1];

			if (j + 2 >= branchVerts.size()) {//need a ghost point if no next index
				p3 = branchVerts[j + 1] + (branchVerts[j + 1] - branchVerts[j]);
			}
			else { 
				p3 = branchVerts[j + 2];
			}

			if (i == 0 && j == 0) {
				cout << "p0: " << p0.transpose() << endl;
				cout << "p1: " << p1.transpose() << endl;
				cout << "p2: " << p2.transpose() << endl;
				cout << "p3: " << p3.transpose() << endl;
			}

			//if chunking, then change k += to .2 or .25, if higher res, then change to .05
			for (float k = 0; k < 1; k += 0.1f) {
				currentBranch.push_back(getPoint(k, p0, p1, p2, p3));
			}
		}
		//needs last point
		currentBranch.push_back(branchVerts.back()); 
		points.push_back(make_pair(currentBranch, branchDepth));
		currentBranch.clear();
	}
	
	return points;
}

Eigen::Vector3f VineGen::getPoint(float t, const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3) {
	//T*M*G
	s = 0.5f;
	Eigen::Matrix4f M;
	M <<
		-s, 2 - s, s - 2, s,
		2 * s, s - 3, 3 - 2 * s, -s,
		-s, 0, s, 0,
		0, 1, 0, 0;
	
	Eigen::Vector4f T;
	T <<  t * t * t, t * t, t, 1.0f;
	Eigen::Matrix<float, 4, 3> G;
	G <<p0.x(), p0.y(), p0.z(),
		p1.x(), p1.y(), p1.z(),
		p2.x(), p2.y(), p2.z(),
		p3.x(), p3.y(), p3.z();

	// 1x4* 4x4 is 1x4, 1x4 * 4x3 is 1x3 of verts
	if ((T.transpose() * M * G).norm() > 1000000.0f) {
		cout << "Error: NaN value in spline calculation!" << endl;
	}

	return (T.transpose() * M * G).transpose();
}