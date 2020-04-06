#pragma once
#include <iostream> 
#include <cmath> 
using namespace std;

// Used to hold details of a point 
struct Coordinate
{
	int x;
	int y;
	Coordinate(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Coordinate()
	{
		x = 0;
		y = 0;
	}
};

// The objects that we want stored in the quadtree 
struct Node
{
	Coordinate pos;
	int data;
	Node(Coordinate _pos, int _data)
	{
		pos = _pos;
		data = _data;
	}
	Node()
	{
		data = 0;
	}
};

// The main quadtree class 
class Quad
{
	// Hold details of the boundary of this node 
	Coordinate topLeft;
	Coordinate botRight;

	// Contains details of node 
	Node* n;

	// Children of this tree 
	Quad* topLeftTree;
	Quad* topRightTree;
	Quad* botLeftTree;
	Quad* botRightTree;

public:
	Quad()
	{
		topLeft = Coordinate(0, 0);
		botRight = Coordinate(0, 0);
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
	}
	Quad(Coordinate topL, Coordinate botR)
	{
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		topLeft = topL;
		botRight = botR;
	}
	void insert(Node*);
	Node* search(Coordinate);
	bool inBoundary(Coordinate);
	void displayTree();
};

// Insert a node into the quadtree 
void Quad::insert(Node* node)
{
	
	if (node == NULL)
		return;

	// Current quad cannot contain it 
	if (!inBoundary(node->pos))
		return;

	// We are at a quad of unit area 
	// We cannot subdivide this quad further 
	if (abs(topLeft.x - botRight.x) <= 1 &&
		abs(topLeft.y - botRight.y) <= 1)
	{
		if (n == NULL)
			n = node;
		cout << "n: (" << n->pos.x << "," << n->pos.y << ") and " << n->data << endl;
		cout << "Insert: (" << node->pos.x << "," << node->pos.y << ") and " << node->data << endl;
		return;
	}

	if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
	{
		// Indicates topLeftTree 
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
		{
			if (topLeftTree == NULL)
				topLeftTree = new Quad(
					Coordinate(topLeft.x, topLeft.y),
					Coordinate((topLeft.x + botRight.x) / 2,
					(topLeft.y + botRight.y) / 2));
			topLeftTree->insert(node);
		}

		// Indicates botLeftTree 
		else
		{
			if (botLeftTree == NULL)
				botLeftTree = new Quad(
					Coordinate(topLeft.x,
					(topLeft.y + botRight.y) / 2),
					Coordinate((topLeft.x + botRight.x) / 2,
						botRight.y));
			botLeftTree->insert(node);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
		{
			if (topRightTree == NULL)
				topRightTree = new Quad(
					Coordinate((topLeft.x + botRight.x) / 2,
						topLeft.y),
					Coordinate(botRight.x,
					(topLeft.y + botRight.y) / 2));
			topRightTree->insert(node);
		}

		// Indicates botRightTree 
		else
		{
			if (botRightTree == NULL)
				botRightTree = new Quad(
					Coordinate((topLeft.x + botRight.x) / 2,
					(topLeft.y + botRight.y) / 2),
					Coordinate(botRight.x, botRight.y));
			botRightTree->insert(node);
		}
	}
}

// Find a node in a quadtree 
Node* Quad::search(Coordinate p)
{
	
	// Current quad cannot contain it 
	if (!inBoundary(p))
		return NULL;

	// We are at a quad of unit length 
	// We cannot subdivide this quad further 
	if (n != NULL)
	{
		cout << "Found: (" << n->pos.x << "," << n->pos.y << ")" << endl;
		return n;
	}

	if ((topLeft.x + botRight.x) / 2 >= p.x)
	{
		
		// Indicates topLeftTree 
		if ((topLeft.y + botRight.y) / 2 >= p.y)
		{
			if (topLeftTree == NULL)
				return NULL;
			return topLeftTree->search(p);
		}

		// Indicates botLeftTree 
		else
		{
			if (botLeftTree == NULL)
				return NULL;
			return botLeftTree->search(p);
		}
	}
	else
	{
		// Indicates topRightTree 
		if ((topLeft.y + botRight.y) / 2 >= p.y)
		{
			if (topRightTree == NULL)
				return NULL;
			return topRightTree->search(p);
		}

		// Indicates botRightTree 
		else
		{
			if (botRightTree == NULL)
				return NULL;
			return botRightTree->search(p);
		}
	}
};

// Check if current quadtree contains the point 
bool Quad::inBoundary(Coordinate p)
{
	return (p.x >= topLeft.x &&
		p.x <= botRight.x &&
		p.y >= topLeft.y &&
		p.y <= botRight.y);
}

void Quad::displayTree()
{

}