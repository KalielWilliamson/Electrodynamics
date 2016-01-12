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
		bool arePointsSame(point cell1, double cell2[3])
		{
			// Compare individual pointinate values
			for (int i = 0; i < k; i++)
				if (cell1.getLocation()[i] != cell2[i])
					return false;
			return true;
		}
		Node* searchRecords(Node* root, double loc[3], unsigned depth)
		{
			// Base cases
			if (root == NULL)
				return NULL;
			if (arePointsSame(root->cell,loc))
				return root;

			// Current dimension is computed using current depth and total
			// dimensions (k)
			unsigned cd = depth % k;

			// Compare point with root with respect to cd (Current dimension)
			if (loc[cd] < root->cell.getLocation()[cd])
				return searchRecords(root->leftChild, loc, ++depth);

			return searchRecords(root->rightChild, loc, depth + 1);
		}
		Node* searchRecordsApprox(Node* root, double cell[3], unsigned depth)
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
					if (cell[cd] < root->cell.getLocation()[cd])
						return searchRecords(root->leftChild, cell, ++depth);

					return searchRecords(root->rightChild, cell, depth + 1);
				}
		Node* search(Node* root, double cell[3])
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
			grid.insert(grid.getRoot(),cell);
		}
		point search(double searchLocation[3])
		{
			Node *getN = grid.search(grid.getRoot(),searchLocation);
			if(getN == NULL)
			{
				point *p = new point();
				return *p;
			}
			return getN->cell;
		}
		point search(double x, double y, double z=0)
		{
			double searchLocation[] = {x,y,z};
			Node *getN = grid.search(grid.getRoot(),searchLocation);
			return getN->cell;
		}
		void setGrid(double distance)
		{
			for(double i=0;i<length;i+=distance)
				{
					for(double j=0;j<width;j+=distance)
					{
						for(double k=0;k<height;k+=distance)
						{
							point p;
							p.setX(i);
							p.setY(j);
							p.setZ(k);
							physics *r = p.getProperties();
							r->setTemp(i+j);
							r->setVector(i,j,k);
							this->add(p);
						}
					}
				}
		}
		void update(point changeCell)
		{
			double dCell[] = {changeCell.getX(),changeCell.getY(),changeCell.getZ()};
			if(dCell[0] <= length && dCell[1] <= width && dCell[2] <= height)
			{
				//find the node in the k-tree with the same location
				Node *getN = grid.search(grid.getRoot(),dCell);
				//update everything else
				getN->cell=changeCell;
			}
		}

		space(int lengthBound, int widthBound, int heightBound=1)
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
	model.setGrid(1);
	for(int i=0;i<length/5;i++)
		{
			for(int j=0;j<width/5;j++)
			{
				point p2 = model.search(i,j);
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

