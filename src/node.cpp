#include "node.h"

#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

using namespace std;

Node::Node() {
	this->id = idCounter++;
	this->parent = nullptr;
	this->children = vector<Node*>();
	this->pos = Eigen::Vector3f(0, 0, 0);
	this->rot = Eigen::Quaternionf(1, 0, 0, 0);
	this->radius = 1;
	this->depth = 0;

}

Node::Node(Eigen::Vector3f pos, Eigen::Quaternionf rot, float radius, int depth, Node& parent) {
	this->id = idCounter++;
	this->parent = &parent;
	this->children = vector<Node*>();
	this->pos = pos;
	this->rot= rot;
	this->radius = radius;
	this->depth = depth;
}

Node ::~Node() {
	for (Node* child : this->children) {
		delete child;
	}

	this->children.clear();
}

Eigen::Vector3f Node::nextPos(float step) {
	this->pos += this->dir * step;
	return this->pos;
}



//need to pass in a vert that will be manipulated
//each new row is a newbranch, prevents ends connecting to begenings
void Node::harvest(vector<vector<Eigen::Vector3f>>& totalBranches, vector<Eigen::Vector3f> currentBranch) {
	
	//adds the verteces to current branch
	currentBranch.push_back(this->getPosition());

	//no children means, add the branch to the total branch list and return
	if (this->children.size() == 0) {
		totalBranches.push_back(currentBranch);
		return;
		//this else if ensures only the first child only adds to current bracnh
	} else if (this->children.size() > 0) {
		this->children[0]->harvest(totalBranches, currentBranch);
	}

	//then it goes to all the oother children to give them their own branch
	for (int i = 1; i < this->children.size(); i++) {
		if (this->children[i] != nullptr) {
			vector<Eigen::Vector3f> newBranch = {this->getPosition()};

			this->children[i]->harvest(totalBranches, newBranch);
		}
	}
}


Eigen ::Vector3f Node::getDirection() {
	return this->dir;
}

void Node::setDirection(Eigen::Vector3f dir) {
	this->dir = dir;
}

void Node::setPosition(Eigen::Vector3f pos) {
	this->pos = pos;
}

Eigen::Vector3f Node::getPosition() {
	return this->pos;
}

void Node::setRotation(Eigen::Quaternionf rot) {
	this->rot = rot;
}

Eigen::Quaternionf Node::getRotation() {
	return this->rot;
}

void Node::setDepth(int depth) {
	this->depth = depth;
}

int Node::getDepth() {
	return this->depth;
}

float Node::getRadius() {
	return this->radius;
}

void Node::setRadius(float radius) {
	this->radius = radius;
}

void Node::addChild(Node* child) {
	this->children.push_back(child);
	child->parent = this;
}

void Node::addParent(Node* parent) {
	this->parent = parent;
	parent->children.push_back(this);
}

Node Node::removeChild(Node* child) {
	auto it = find(this->children.begin(), this->children.end(), child);
	if (it != this->children.end()) {
		this->children.erase(it);
		child->parent = nullptr;
		return *child;
	}
	throw runtime_error("Child not found");
}

Node Node::removeParent(Node* parent) {
	if (this->parent == parent) {
		this->parent = nullptr;
		auto it = find(parent->children.begin(), parent->children.end(), this);
		if (it != parent->children.end()) {
			parent->children.erase(it);
		}
		return *parent;
	}
	throw runtime_error("Parent not found");
}

