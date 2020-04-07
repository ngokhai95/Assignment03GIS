#pragma once
#include <iostream> 
#include <cmath> 
using namespace std;

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

class Quad
{
	Coordinate topLeft;
	Coordinate botRight;

	Node* n;

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
	void displayTree(Quad* tree);
};

// Insert a node into the quadtree 
void Quad::insert(Node* node)
{
	
	if (node == NULL)
		return;

	//outside boundary
	if (!inBoundary(node->pos))
		return;

	//cannot be divided further
	if (abs(topLeft.x - botRight.x) <= 1 &&
		abs(topLeft.y - botRight.y) <= 1)
	{
		if (n == NULL)
			n = node;
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
	// Outside boundary
	if (!inBoundary(p))
		return NULL;

	//cannot be divided further
	if (n != NULL)
	{
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

// Check if point is in boundary
bool Quad::inBoundary(Coordinate p)
{
	return (p.x >= topLeft.x &&
		p.x <= botRight.x &&
		p.y >= topLeft.y &&
		p.y <= botRight.y);
}

void Quad::displayTree(Quad* tree)
{
	if (tree->n != NULL)
	{
		cout << "[(" << tree->n->pos.x << "," << tree->n->pos.y << "), " << tree->n->data << "]" << endl;
	}
	if (tree->topLeftTree != NULL)
	{
		displayTree(tree->topLeftTree);
	}
	if (tree->topRightTree != NULL)
	{
		displayTree(tree->topRightTree);
	}
	if (tree->botLeftTree != NULL)
	{
		displayTree(tree->botLeftTree);
	}
	if (tree->botRightTree != NULL)
	{
		displayTree(tree->botRightTree);
	}
}