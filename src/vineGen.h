#pragma once

#include "vine.h"
#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <cmath>
#include <unordered_map>
#include <iostream>

using namespace std;

class VineGen
{
	private : 
		int iterations;
		string start;
		unordered_map<string, string> rules;
		vector<vector<Eigen::Vector3f>> verts;

		//helpers for SplineIt
		 //spline cntrl points
		Eigen::Vector3f p0;
		Eigen::Vector3f	p1;
		Eigen::Vector3f p2; 
		Eigen::Vector3f p3; 
		 //for the M matrix
		float s = 1.0f / 2.0f;

public: 
	VineGen(int iterations, string start, unordered_map<string, string> rules);
	string getLString(int iterations, string start, unordered_map<string, string> rules);
	vector<vector<Eigen::Vector3f>> generate_vine(string LString);
	vector<vector<Eigen::Vector3f>> splineIt();

	//helper functions for splineIt
	Eigen::Vector3f getPoint(float t);
};