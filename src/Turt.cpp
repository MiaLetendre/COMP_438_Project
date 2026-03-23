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
	root = new Node();
	currentNode = root;
	heading = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	rads = (22.5f * static_cast<float>(pi) / 180.0f);
}

Turt::Turt(Node* rootNode, float angle) {
	root = rootNode;
	currentNode = root;
	heading = Eigen::Vector3f(0.0f, 1.0f, 0.0f);
	rads = (angle * static_cast<float>(pi) / 180.0f);
}


//currently lang is F, +, -, [, ]
//want to add ^ and & to go upand down on y axis, and < and > to roll left and right on z axis
//this will need a local uo,right and forward vector
//also maybe I need to add a X dummy var to ignore
// i think maybe addidn a L and a B for leaf and Bloom/flower (F was already used) 
void Turt::processString(const string& commands) {
	Eigen::Matrix3f twisty;
	Flower flower;
	Leaf leaf;
	for (char command : commands) {
		switch (command) {
			case 'F':
				//default step size of 1.0
				stepForward(1.0f); 
				break;
			case 'G':
				stepForward(1.0f);
				break;
			case 'H':
				stepForward(1.0f);
				break;
			case 'I':
				stepForward(1.0f);
				break;
			case 'J':
				stepForward(1.0f);
				break;
			case '[':
				push();
				break;
			case ']':
				pop();
				break;
			case '+':
				//rotate right by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitZ());
				heading = twisty * heading; 
				break;
			case '-':
				//rotate left by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitZ());
				heading = twisty.transpose() * heading; 
				break;
			case '^':
				//rotate up by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitX());
				heading = twisty * heading; 
				break;
			case '&':
				//rotate down by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitX());
				heading = twisty.transpose() * heading; 
				break;
			case '<':
				//roll left by 45 degrees
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitY());
				heading = twisty * heading; 
				break;
			case '>':
				twisty = Eigen::AngleAxisf(rads, Eigen::Vector3f::UnitY());
				heading = twisty.transpose() * heading; //roll right by 45 degrees
				break;
			
			/* could be how i add leafs/flowers, or jsut edit python file
			case 'L':
				//uhhh how do i implement leaf?
				leaf.leafHead = this->heading;
				leaf.leafPos = this->currentNode->getPosition();
				break;
			case 'B':
				//dido
				
				flower.flowerHead = this->heading;
				flower.flowerPos = this->currentNode->getPosition();
				break;
				*/
			case 'X':
				//ignored like other things, but I still want it there 
				break;
			case 'Y':
				//ignored like other things, but I still want it there 
				break;
			case 'Z':
				//ignored like other things, but I still want it there 
				break;
			default:
				//ignore unrecognized commands
				break;
		}
	}
}

void Turt::stepForward(float step) {
	//this fucntion is the basis on how ass the nodes are added to the tree
	//how this works is that it find the next positions that the node will go, makes the roation vec equal to the rotation, then creates a new node in that direction
	Eigen::Vector3f newPos = currentNode->nextPos(step, heading);
	Eigen::Quaternionf rot = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f::UnitZ(), this->heading);
	Node* newNode = new Node(newPos, rot, currentNode->getRadius(), this->currentNode->getDepth() + 1, *this->currentNode);
	//makes the new node this nodes child which is important relationsally for harvesting, thenn changes to that ndoe
	this->currentNode->addChild(newNode);
	this->currentNode = newNode;
}

void Turt::push() {
	prevState state = {currentNode, heading };
	nodeStack.push(state);
}

void Turt::pop() {
	if (nodeStack.empty()) {
		std::cout<< "empty node stack" <<endl;
		return;
	}
	prevState state = nodeStack.top();
	currentNode = state.node;
	heading = state.heading;
	nodeStack.pop();
}

Node* Turt::getRoot() {
	return root;
}

void Turt::setRoot(Node* rootNode) {
	root = rootNode;
}

Node* Turt::getCurrentNode() {
	return currentNode;
}

void Turt::setCurrentNode(Node* currentNode) {
	this->currentNode = currentNode;
}

Eigen::Vector3f Turt::getHeading() {
	return heading;
}

void Turt::setHeading(Eigen::Vector3f heading) {
	this->heading = heading;
}