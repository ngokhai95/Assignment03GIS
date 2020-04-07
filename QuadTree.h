#pragma once
#include <iostream> 
#include <cmath> 
using namespace std;

int times = 2;

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
	vector<int> bucketOffset;
	Node(Coordinate _pos, int _data)
	{
		pos = _pos;
		bucketOffset.push_back(_data);
	}
	Node()
	{
		bucketOffset.push_back(0);
	}
};

class Quad
{
	Coordinate northWest;
	Coordinate southEast;

	Node* node;

	Quad* northWestArea;
	Quad* northEastArea;
	Quad* SouthWestArea;
	Quad* southEastArea;

public:
	Quad()
	{
		northWest = Coordinate(0, 0);
		southEast = Coordinate(0, 0);
		node = NULL;
		northWestArea = NULL;
		northEastArea = NULL;
		SouthWestArea = NULL;
		southEastArea = NULL;
	}
	Quad(Coordinate topL, Coordinate botR)
	{
		node = NULL;
		northWestArea = NULL;
		northEastArea = NULL;
		SouthWestArea = NULL;
		southEastArea = NULL;
		northWest = topL;
		southEast = botR;
	}
	void insert(Node*);
	Node* search(Coordinate);
	bool inBoundary(Coordinate);
	void displayTree(Quad* tree);
};

// Insert a node into the quadtree 
void Quad::insert(Node* input)
{
	
	if (input == NULL)
		return;

	//outside boundary
	if (!inBoundary(input->pos))
		return;

	//cannot be divided further
	if (abs(northWest.x - southEast.x) <= 1 &&
		abs(northWest.y - southEast.y) <= 1)
	{
		if (node == NULL) //empty node
		{
			node = input;
		}
		else //if node exist, search for it then add input offset to the node's offset bucket
		{
			this->search(input->pos)->bucketOffset.push_back(input->bucketOffset[0]);
		}
		return;
	}

	if ((northWest.x + southEast.x) / 2 >= input->pos.x)
	{
		// Traverse northWestArea 
		if ((northWest.y + southEast.y) / 2 >= input->pos.y)
		{
			if (northWestArea == NULL)
				northWestArea = new Quad(
					Coordinate(northWest.x, northWest.y),
					Coordinate((northWest.x + southEast.x) / 2,
					(northWest.y + southEast.y) / 2));
			northWestArea->insert(input);
		}

		// Traverse southWestArea 
		else
		{
			if (SouthWestArea == NULL)
				SouthWestArea = new Quad(
					Coordinate(northWest.x,
					(northWest.y + southEast.y) / 2),
					Coordinate((northWest.x + southEast.x) / 2,
						southEast.y));
			SouthWestArea->insert(input);
		}
	}
	else
	{
		// Traverse northEastArea  
		if ((northWest.y + southEast.y) / 2 >= input->pos.y)
		{
			if (northEastArea == NULL)
				northEastArea = new Quad(
					Coordinate((northWest.x + southEast.x) / 2,
						northWest.y),
					Coordinate(southEast.x,
					(northWest.y + southEast.y) / 2));
			northEastArea->insert(input);
		}

		// Traverse southEastArea 
		else
		{
			if (southEastArea == NULL)
				southEastArea = new Quad(
					Coordinate((northWest.x + southEast.x) / 2,
					(northWest.y + southEast.y) / 2),
					Coordinate(southEast.x, southEast.y));
			southEastArea->insert(input);
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
	if (node != NULL)
	{
		return node;
	}

	if ((northWest.x + southEast.x) / 2 >= p.x)
	{
		
		// Traverse northWestArea 
		if ((northWest.y + southEast.y) / 2 >= p.y)
		{
			if (northWestArea == NULL)
				return NULL;
			return northWestArea->search(p);
		}

		// Traverse southWestArea 
		else
		{
			if (SouthWestArea == NULL)
				return NULL;
			return SouthWestArea->search(p);
		}
	}
	else
	{
		// Traverse northEastArea  
		if ((northWest.y + southEast.y) / 2 >= p.y)
		{
			if (northEastArea == NULL)
				return NULL;
			return northEastArea->search(p);
		}

		// Traverse southEastArea 
		else
		{
			if (southEastArea == NULL)
				return NULL;
			return southEastArea->search(p);
		}
	}
};

// Check if point is in boundary
bool Quad::inBoundary(Coordinate p)
{
	return (p.x >= northWest.x &&
		p.x <= southEast.x &&
		p.y >= northWest.y &&
		p.y <= southEast.y);
}

void Quad::displayTree(Quad* tree)
{

	if (times > 1)
	{
		cout << endl << "@" << endl;
		times = 0;
	}
	if (tree->node != NULL)
	{
		cout << "[(" << tree->node->pos.x << "," << tree->node->pos.y << ")";
		for (auto offset : tree->node->bucketOffset)
		{
			cout << ", ";
			cout << offset;
		}
		cout << "]";
	}
	
	
	if (tree->northWestArea != NULL)
	{
		displayTree(tree->northWestArea);
	}
	if (tree->northEastArea != NULL)
	{
		displayTree(tree->northEastArea);
		if (tree->northWestArea == NULL && tree->SouthWestArea == NULL && tree->southEastArea == NULL)
		{
			times++;
			cout << endl;
			if (times > 1)
			{
				cout << "*";
			}
		}
	}
	if (tree->SouthWestArea != NULL)
	{
		displayTree(tree->SouthWestArea);
	}
	if (tree->southEastArea != NULL)
	{
		displayTree(tree->southEastArea);
		
	}
}