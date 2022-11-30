#pragma once
#include <vector>
#include "Node.h"
class MinHeap
{
private:
	std::vector<Node*> nodeList;
	Node* child(int index);

public:
	MinHeap();
	//gets the index of a node in the heap, returns -1 if not in the heap
	int getIndex(Node* i);
	//returns the smallest node on the heap and removes it, heapifys the rest of the nodes
	Node* extractMin();
	//returns the smallest node on the heap
	Node* minNode();
	//returns the amount of nodes on the heap
	int size();
	//adds a node to the heap
	void insert(Node* node);
	//shuffles node at the index down 
	void heapify(int index);
};

