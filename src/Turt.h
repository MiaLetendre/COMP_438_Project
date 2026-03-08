#pragma once

#include "node.h"
#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <stack>

using namespace std;


class Turt
{
public: 

	

	Turt(); //default constructor
	Turt(Node* rootNode); //constructor with root node
	void processString(const string& commands); //processes a string of commands to generate the vine structure
	void stepForward(float step); //translates 'F' 
	void push(); //translates '['
	void pop(); //translates ']'

	Node* getRoot();
	void setRoot(Node* rootNode);\
	Node* getCurrentNode();
	void setCurrentNode(Node* currentNode);
	Eigen::Vector3f getHeading();
	void setHeading(Eigen::Vector3f heading);



private:

	struct prevState {
		Node* node;
		Eigen::Vector3f heading;
	};
	Node* currentNode; //current node of the turtle
	Node* root; //root node of the turtle
	Eigen::Vector3f heading; //rotation vector for '+' and '-' commands
	stack<prevState> nodeStack; //stack to keep track of nodes for push and pop operations
};