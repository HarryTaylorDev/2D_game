#include "minHeap.h"
#include <algorithm>
#include <stdio.h>
#include <iostream>

Node* MinHeap::child(int index)
{ 
	//returns the smaller child
	if (nodeList[(index * 2) - 1]->getF() < nodeList[index * 2]->getF()) {
		return nodeList[(index * 2) - 1];
	}
	else {
		return nodeList[index * 2];
	}
}

MinHeap::MinHeap()
{
}

int MinHeap::getIndex(Node* i)
{
	//search for the node in the heap
	std::vector<Node*>::iterator iter = std::find(nodeList.begin(), nodeList.end(), i);

	//if iterator has not reached the end of the heap
	if (iter != nodeList.end())
	{
		//return the index of the node
		return std::distance(nodeList.begin(), iter);
	}
	else {
		//return -1 if the node is not in the heap
		return -1;
	}
}

Node* MinHeap::extractMin()
{
	//get the smallest node
	Node* result = nodeList[0];
	//move the node at the end of the heap to the front
	nodeList[0] = nodeList.back();
	//remove the node from the end
	nodeList.pop_back();

	int i = 1;

	//shuffle the node starting at the stop down until it is smaller that both its children
	while ((2 * i < nodeList.size()) && (nodeList[i - 1]->getF() > child(i)->getF())) {
		int j = i;
		i = 1 + getIndex(child(i));
		std::swap(nodeList[j - 1], nodeList[getIndex(child(j))]);
	}

	// completes the move to the bottom layer if the
	// node only has one child
	if (2 * i == nodeList.size() && (nodeList[i - 1]->getF() > nodeList[(2 * i) - 1]->getF())) {
		std::swap(nodeList[i - 1], nodeList[(2*i)-1]);
	}

	

	return result;
}

Node* MinHeap::minNode()
{
	//returns the root node
	return nodeList[0];
}

int MinHeap::size()
{
	//returns the size of the heap
	return nodeList.size();
}

void MinHeap::insert(Node* node)
{
	//adds the node to the back of the heap
	nodeList.push_back(node);
	//shuffles the nodes to make sure the heap is still order correctly
	heapify(nodeList.size());

}

void MinHeap::heapify(int index)
{
	//Moves a node up through the heap when
	//it is either added or has its cost updated
	while (index > 1 && nodeList[index - 1]->getF() < nodeList[((index / 2) - 1)]->getF()) {
		std::swap(nodeList[index - 1],nodeList[((index / 2) - 1)]);
		index = index / 2;
	}
}
