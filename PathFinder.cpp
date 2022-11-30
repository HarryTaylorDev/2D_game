#include "PathFinder.h"
#include <stdio.h>
#include <iostream>

PathFinder::PathFinder() {
}

PathFinder::PathFinder(Level& level)
{
	//generate the 2d array of nodes
	for (int y = 0; y < level.tiles.size(); y++) {
		nodeList.push_back({});
		for (int x = 0; x < level.tiles[y].size(); x++) {
			nodeList[y].push_back(Node(x, y, !level.tiles[y][x]->collidable));
		}
	}
}

void PathFinder::resetNodeList()
{
	//reset the list of nodes
	for (int y = 0; y < nodeList.size(); y++) {
		for (int x = 0; x < nodeList[y].size(); x++) {
			nodeList[y][x].g = 0.0;
			nodeList[y][x].explored = false;
			nodeList[y][x].parent = nullptr;
		}
	}
}

float PathFinder::Distance(wayPoint current)
{
	//calcualte the heuristic, manhattan distance
	float dx = current.x - end.x;
	float dy = current.y - end.y;

	return (abs(dx) + abs(dy));
}

std::vector<Node*> PathFinder::getNeighbours(Node* current)
{
	//returns a list of the nodes adjacent nodes
	std::vector<Node*> result;
	
	if (current->coords.y > 0) {
		result.push_back(&nodeList[current->coords.y - 1][current->coords.x]);
	}

	if (current->coords.x > 0) {
		result.push_back(&nodeList[current->coords.y][current->coords.x - 1]);
	}

	if (current->coords.y < nodeList.size() - 1) {
		result.push_back(&nodeList[current->coords.y + 1][current->coords.x]);		
	}

	if (current->coords.x < nodeList.size() - 1) {
		result.push_back(&nodeList[current->coords.y][current->coords.x + 1]);
	}

	return result;
}

std::vector<wayPoint> PathFinder::getPath(wayPoint _start, wayPoint _end)
{
	//make the start and end way points
	int y = nodeList.size() - 1 - (int)_start.y;
	start = { _start.x, y };
	y = nodeList.size() - 1 - (int)_end.y;
	end = { _end.x,y };

	//add the start node to the open list
	openList.insert(&nodeList[start.y][start.x]);
	//update the start node
	nodeList[start.y][start.x].h = Distance(nodeList[start.y][start.x].coords);
	nodeList[start.y][start.x].g = 0.0;
	
	//loop through the open list until it is empty
	while (openList.size() > 0) {
		//get the smallest node
		Node* current = openList.extractMin();

		//set the current node as explored
		current->explored = true;

		//exit if the current node is the end waypoint, this means a path has been found
		if (current->coords.x == end.x && current->coords.y == end.y) {
			break;
		}

		//get the current nodes neighbours
		std::vector<Node*> neighbours = getNeighbours(current);
		//loop though all neighbours
		for (auto neighbour : neighbours) {
			//update the node if it is traversable and not explored
			if (neighbour->traversable && !neighbour->explored) {
				//check if the new path to the neighbour is shorter
				if (current->g + 1 < neighbour->g) {
					//update the node to new node
					neighbour->parent = current;
					neighbour->g = current->g + 1;
					//calculate the heuristic
					if (!(neighbour->h > 0.0)) {
						neighbour->h = Distance(neighbour->coords);
					}
					
					//check if the neighbour is in the open list
					int index = openList.getIndex(neighbour);
					//add it if it is not in the open list
					if (index == -1) {
						openList.insert(neighbour);
					}
					//else update its position in the heap
					else {
						openList.heapify(index + 1);
					}
				}
			}
		}
	}

	std::vector<wayPoint> result;

	//produce the list of way points from the path in the nodes
	Node* next = &nodeList[end.y][end.x];
	if (next->parent != nullptr) {
		while ((next != &nodeList[start.y][start.x] && next != nullptr)|| next != nullptr) {
			wayPoint temp = { next->coords.x, nodeList.size() - next->coords.y -1};
			result.push_back(temp);
			next = next->parent;
		}
	}

	//Code to print out the path made and then the area searched

	/*bool test = false;
	for (int y = 0; y < nodeList.size(); y++) {
		for (int x = 0; x < nodeList[y].size(); x++) {
			for (wayPoint w : result) {
				if (y == (w.y - nodeList.size() + 1) && x == w.x) {
					test = true;
				}
			}

			if (test) {
				std::cout << "X";
			}
			else {
				std::cout << nodeList[y][x].traversable;
			}
			test = false;
		}
		std::cout << std::endl;
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	for (int y = 0; y < nodeList.size(); y++) {
		for (int x = 0; x < nodeList[y].size(); x++) {
			std::cout << nodeList[y][x].explored;
		}
		std::cout << std::endl;
	}*/

	return result;
}




