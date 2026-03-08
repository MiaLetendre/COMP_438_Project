#include "Turt.h"
#include "node.h"
#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <stack>
#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>

using namespace std;



Turt::Turt() {
	this->root = new Node();
	this->currentNode = this->root;
	this->heading = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	
}

Turt::Turt(Node* rootNode) {
	this->root = rootNode;
	this->currentNode = this->root;
}


//currently lang is F, +, -, [, ]
//want to add ^ and & to go upand down on y axis, and < and > to roll left and right on z axis
//this will need a local uo,right and forward vector
//also I need to add a X dummy var to ignore

void Turt::processString(const string& commands) {
	Eigen::Matrix3f twisty;
	for (char command : commands) {
		switch (command) {
			case 'F':
				//default step size of 1.0
				this->stepForward(1.0f); 
				break;
			case '[':
				this->push();
				break;
			case ']':
				this->pop();
				break;
			case '+':
				//rotate right by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitZ());
				this->heading = twisty * this->heading; 
				break;
			case '-':
				//rotate left by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitZ());
				this->heading = twisty.transpose() * this->heading; 
				break;
			case '^':
				//rotate up by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitX());
				this->heading = twisty * this->heading; 
				break;
			case '&':
				//rotate down by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitX());
				this->heading = twisty.transpose() * this->heading; 
				break;
			case '<':
				//roll left by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitY());
				this->heading = twisty * this->heading; 
				break;
			case '>':
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitY());
				this->heading = twisty.transpose() * this->heading; //roll right by 45 degrees
			default:
				//ignore unrecognized commands
				break;
		}
	}
}

void Turt::stepForward(float step) {
	Eigen::Vector3f newPos = this->currentNode->nextPos(step, this->heading);
	Eigen::Quaternionf rot = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), this->heading);

	Node* newNode = new Node(newPos, rot, this->currentNode->getRadius(), this->currentNode->getDepth() + 1, *this->currentNode);
	
	this->currentNode->addChild(newNode);
	this->currentNode = newNode;
}

void Turt::push() {
	prevState state = { this->currentNode, this->heading };
	this->nodeStack.push(state);
}

void Turt::pop() {
	if (this->nodeStack.empty()) {
		std::cout<< "empty node stack" <<endl;
		return;
	}
	prevState state = this->nodeStack.top();
	this->currentNode = state.node;
	this->heading = state.heading;
	this->nodeStack.pop();
}

Node* Turt::getRoot() {
	return this->root;
}

void Turt::setRoot(Node* rootNode) {
	this->root = rootNode;
}

Node* Turt::getCurrentNode() {
	return this->currentNode;
}

void Turt::setCurrentNode(Node* currentNode) {
	this->currentNode = currentNode;
}

Eigen::Vector3f Turt::getHeading() {
	return this->heading;
}

void Turt::setHeading(Eigen::Vector3f heading) {
	this->heading = heading;
}