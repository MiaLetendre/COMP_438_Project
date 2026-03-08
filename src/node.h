#pragma once

#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

using namespace std;

static int idCounter = 0; //global variable to assign unique ids to nodes

class Node
{
public:
	Node(); //default constructor
	Node(Eigen::Vector3f pos, Eigen::Quaternionf rot, float radius, int depth, Node& parent);
	~Node(); //destructor
	Eigen::Vector3f nextPos(float step, Eigen::Vector3f heading); //multiplies step by direction and adds to pos
	
	//needs to be a stack
	//push saves pos, pop restores pos
	void harvest(vector<vector<Eigen::Vector3f>>& totalBranches, vector<Eigen::Vector3f> currentBranch); //here come the reaper!! (gets the vertex data for blender)
	
	
	Eigen::Vector3f getDirection(); //returns the direction of this node
	void setDirection(Eigen::Vector3f dir); //sets the direction of this node
	void setPosition(Eigen::Vector3f pos); //sets the position of this node
	Eigen::Vector3f getPosition(); //returns the position of this node
	void setRotation(Eigen::Quaternionf rot); //sets the rotation of this node
	Eigen::Quaternionf getRotation(); //returns the rotation of this node
	void setDepth(int depth); //sets the depth of this node
	int getDepth(); //returns the depth of this node
	float getRadius(); //returns the radius of this node
	void setRadius(float radius); //sets the radius of this node
	void addChild(Node* child); //adds a child, parents this node to child
	void addParent(Node* parent); //adds a parent, children this node to parent
	Node removeChild(Node* child); //removes a child, unparents this node to child
	Node removeParent(Node* parent); //removes a parent, unchildren this node to parent

private:
	int id; //identifies this node
	Eigen::Vector3f pos; //position
	Eigen::Vector3f dir; //direction
	Eigen::Quaternionf rot; //rotation
	float radius; //how big the vine is
	int depth; //tracks how deep this node is on the vine
	vector<Node*> children; //children of this node
	Node* parent; //parent of this node


	


};