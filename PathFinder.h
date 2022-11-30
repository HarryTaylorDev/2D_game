#pragma once
#include "Node.h"
#include <stack> 
#include "Level.h"
#include <math.h>
#include <limits.h>
#include "MinHeap.h"



class PathFinder
{
private:
	//returns the distance from current node to passed in coordinates
	float Distance(wayPoint current);
	//returns a list of the neighbours of the current node
	std::vector<Node*> getNeighbours(Node* current);

public:
	wayPoint start;
	wayPoint end;

	//2d array of nodes
	std::vector<std::vector<Node>> nodeList;

	//priority queue of nodes (minimum heap)
	MinHeap openList;
	
	PathFinder();
	//generates the 2d array of nodes
	PathFinder(Level &level);
	
	//returns a list of points in the shortest path from the 
	//start to destination
	std::vector<wayPoint> getPath(wayPoint start, wayPoint end);
	//reset the 2d array of nodes to default values
	void resetNodeList();

};

