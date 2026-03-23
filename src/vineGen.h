#pragma once

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
		vector<pair<vector<Eigen::Vector3f>, int>> verts;

		//helpers for SplineIt
		 //spline cntrl points
		Eigen::Vector3f p0;
		Eigen::Vector3f	p1;
		Eigen::Vector3f p2; 
		Eigen::Vector3f p3; 
		 //for the M matrix
		float s = 0.5f;

public: 
	VineGen(int iterations, string start, unordered_map<string, string> rules);
	string getLString(int iterations, string start, unordered_map<string, string> rules);
	vector<pair<vector<Eigen::Vector3f>, int>> generateVine(string LString, float angle);
	vector<pair<vector<Eigen::Vector3f>, int>> splineIt();
	//helper functions for splineIt
	Eigen::Vector3f getPoint(float t, const Eigen::Vector3f& p0, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2, const Eigen::Vector3f& p3);
	};