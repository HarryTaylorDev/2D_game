#pragma once

//pair of coordinates, used to pass around a point easier
struct wayPoint {
	int x;
	int y;
};

class Node
{
public:
	// cost from start
	float g;
	// estimated distance to destination
	float h; 
	Node* parent;
	bool explored;
	bool traversable;
	wayPoint coords;

	//creates a node
	Node(int x,int y,bool _traversable);
	//gets the f cost of the node (g+h)
	const float getF() const;
};

