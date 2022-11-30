#include "Node.h"
#include <limits>

Node::Node(int x,int y, bool _traversable)
{
	//set up the nodes values
	g = std::numeric_limits<float>::infinity();
	h = 0.0;
	explored = false;
	traversable = _traversable;
	coords = { x, y };
	parent = nullptr;
}


const float Node::getF() const
{
	//return the f cost which is the g cost plus the heuristic
	return g + h;
}