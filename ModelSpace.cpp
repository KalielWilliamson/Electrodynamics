/*
 * ModelSpace.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: Kaliel
 */
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include "Points.cpp"
#include "Physics.cpp"
class Node
{
	public:
		point cell;
		Node *leftChild;
		Node *rightChild;
		Node(point newCell)
		{
			cell = newCell;
			leftChild = NULL;
			rightChild = NULL;
		}
};
class KTree
{

	private:
		const int k = 2;
		Node *_root;
	public:
		Node *insertRec(Node *root, point cell, unsigned depth)
		{
			// Tree is empty?
			if (root == NULL)
			   return new Node(cell);

			// Calculate current dimension (cd) of comparison
			unsigned cd = depth % k;

			// Compare the new point with root on current dimension 'cd'
			// and decide the left or right subtree
			double* firstLoc = cell.getLocation();
			double* secondLoc = root->cell.getLocation();
			if (firstLoc[cd] < (secondLoc[cd]))
				root->leftChild  = insertRec(root->leftChild, cell, depth + 1);
			else
				root->rightChild = insertRec(root->rightChild, cell, depth + 1);

			return root;
		}
		Node* insert(Node *root, point cell)
		{
			return insertRec(root, cell, 0);
		}
		Node* getRoot()
		{
			return this->_root;
		}
		bool arePointsSame(point cell1, point cell2)
		{
			// Compare individual pointinate values
			for (int i = 0; i < k; ++i)
				if (cell1.getLocation()[i] != cell2.getLocation()[i])
					return false;
			return true;
		}
		Node* searchRecords(Node* root, point cell, unsigned depth)
		{
			// Base cases
			if (root == NULL)
				return NULL;
			if (arePointsSame(root->cell,cell))
				return root;

			// Current dimension is computed using current depth and total
			// dimensions (k)
			unsigned cd = depth % k;

			// Compare point with root with respect to cd (Current dimension)
			if (cell.getLocation()[cd] < root->cell.getLocation()[cd])
				return searchRecords(root->leftChild, cell, ++depth);

			return searchRecords(root->rightChild, cell, depth + 1);
		}
		Node* searchRecordsApprox(Node* root, point cell, unsigned depth)
				{
					// Base cases
					if (root == NULL)
						return NULL;
					if (arePointsSame(root->cell,cell))
						return root;

					// Current dimension is computed using current depth and total
					// dimensions (k)
					unsigned cd = depth % k;

					// Compare point with root with respect to cd (Current dimension)
					if (cell.getLocation()[cd] < root->cell.getLocation()[cd])
						return searchRecords(root->leftChild, cell, ++depth);

					return searchRecords(root->rightChild, cell, depth + 1);
				}
		Node* search(Node* root, point cell)
		{
			// Pass current depth as 0
			return searchRecords(root, cell, 0);
		}
		KTree()
		{
			point first;
			_root = new Node(first);
		}
		~KTree(){}
};

//create a model 2D space where
class space
{
	private:
		int length;
		int width;
		int height;
		KTree grid; //2D k-search tree of points
	public:
		void setWidth(int w)
		{
			width = w;
		}
		void setLength(int l)
		{
			length = l;
		}
		void setHeight(int h)
		{
			height = h;
		}
		void add(point cell)
		{
			if(cell.getX() <= length && cell.getY() <= width && cell.getZ() <= height)
			{
				grid.insert(grid.getRoot(),cell);
			}
		}
		point search(point searchPoint)
		{
			Node *getN = grid.search(grid.getRoot(),searchPoint);
			return getN->cell;
		}
		point search(double x, double y, double z)
		{
			point p;
			p.setX(x);
			p.setY(y);
			p.setZ(z);
			Node *getN = grid.search(grid.getRoot(),p);
			return getN->cell;
		}
		void update(point changeCell)
		{
			if(changeCell.getX() <= length && changeCell.getY() <= width && changeCell.getZ() <= height)
			{
				//find the node in the k-tree with the same location
				Node *getN = grid.search(grid.getRoot(),changeCell);
				//update everything else
				getN->cell=changeCell;
			}
		}

		space(int lengthBound, int widthBound, int heightBound=0)
		{
			setLength(lengthBound);
			setWidth(widthBound);
			setHeight(heightBound);
		}
		~space(){}
};

int main()
{
	int length = 100;
	int width = 100;
	double x;
	double y;
	double z;
	double temp;
	space model(length,width);
	point cells[length][width];
	for(int i=0;i<length;i++)
	{
		for(int j=0;j<width;j++)
		{
			point p;
			p.setX(i);
			p.setY(j);
			p.setZ(0);
			physics *r = p.getProperties();
			r->setTemp(i+j);
			r->setVector(i,j,3);
			cells[i][j] = p;
			model.add(p);
		}
	}
	for(int i=0;i<length/5;i++)
		{
			for(int j=0;j<width/5;j++)
			{
				point p1;
				p1.setX(i);
				p1.setY(j);
				point p2 = model.search(p1);
				physics *r = p2.getProperties();
				temp = r->getTemp();
				x = r->getX();
				y = r->getY();
				z = r->getZ();
				std::cout <<"("<< p2.getX() <<","<<p2.getY()<<","<<p2.getZ()<<"): T-"<<temp<<" P("<<
						x<<","<<y<<","<<z<<")"<<std::endl;
			}
		}
}

