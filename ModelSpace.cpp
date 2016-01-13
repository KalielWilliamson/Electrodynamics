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
#include <stack>
#include "Points.cpp"
#include "Physics.cpp"
class Node
{
	public:
		point cell;
		Node *parent;
		Node *leftChild;
		Node *rightChild;
		Node(point newCell)
		{
			cell = newCell;
			parent = NULL;
			leftChild = NULL;
			rightChild = NULL;
		}
		Node(Node *previous,point newCell)
		{
			cell = newCell;
			parent = previous;
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
			   return new Node(root,cell);

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
				return root->parent;
			if (arePointsSame(root->cell,loc))
				return root;

			// Current dimension is computed using current depth and total
			// dimensions (k)
			int cd = depth % k;
			bool a = loc[cd] < root->cell.getLocation()[cd];
			bool b=false;
			bool c=false;
			bool d=false;
			if(root->leftChild != NULL)
				b = loc[cd] > root->leftChild->cell.getLocation()[cd];
			if(root != NULL)
				c = loc[cd] > root->cell.getLocation()[cd];
			if(root->rightChild != NULL)
				d = loc[cd] < root->rightChild->cell.getLocation()[cd];
			if((a && b) || (c && d))
				return root;

			// Compare point with root with respect to cd (Current dimension)
			if(root->leftChild == NULL && root->rightChild == NULL)
			{
				return root;
			}
			if (a)
				return searchRecords(root->leftChild, loc, ++depth);
			if (loc[cd] >= root->cell.getLocation()[cd])
			{
				return searchRecords(root->rightChild, loc, ++depth);
			}
			if(cd == 1)
				std::cout <<"found 3"<<std::endl;

			//return root;
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
		double length;
		double width;
		double height;
		double gridResolution;
		point grid[100][100][100];
		KTree mesh; //2D k-search tree of points
	public:
		void setWidth(double w)
		{
			width = w;
		}
		void setLength(double l)
		{
			length = l;
		}
		void setHeight(double h)
		{
			height = h;
		}
		void add(point cell)
		{
			mesh.insert(mesh.getRoot(),cell);
		}
		point search(double searchLocation[3])
		{
			Node *getN = mesh.search(mesh.getRoot(),searchLocation);
			if(getN == NULL)
			{
				point *p = new point();
				return *p;
			}
			return getN->cell;
		}
		point search(double x, double y, double z)
		{
			double searchLocation[] = {x,y,z};
			Node *getN = mesh.search(mesh.getRoot(),searchLocation);
			return getN->cell;
		}
		void setMesh(double distance)
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
				Node *getN = mesh.search(mesh.getRoot(),dCell);
				//update everything else
				getN->cell=changeCell;
			}
		}
		Node* getChild(Node *N,int lr)
		{
			//base case
			if(N->leftChild == NULL && N->rightChild == NULL)
				return NULL;
			if(N->leftChild != NULL && lr == 1)
			{
				return N->leftChild;
			}
			else
			{
				return N->rightChild;
			}
			if(N->leftChild != NULL && lr == 2)
			{
				return N->rightChild;
			}
			else
			{
				return N->leftChild;
			}

		}
		physics* getClosest(double loc[3])
		{
			//method for finding closest 3 nodes on tree and returning physics point
			//grab node
			int ind = 0;
			physics *closest[4];
			Node *getN = mesh.search(mesh.getRoot(),loc);
			closest[0] = getN->cell.getProperties();
			closest[1] = getN->parent->cell.getProperties();
			closest[2] = getN->leftChild->cell.getProperties();
			closest[3] = getN->rightChild->cell.getProperties();
			for(int i=0;i<4;i++)
				if(closest[i] != NULL)
					ind++;
			physics *clo[ind];
			ind=0;
			for(int i=0;i<4;i++)
					if(closest[i] != NULL)
						clo[ind++]=closest[i];

			return *clo;
		}
		void setGridResolution(double res)
		{
			gridResolution = res;
		}
		void interpolateGrid()
		{
			point *p1;
			double temp;
			for(int i=0;i<length*gridResolution;i++)
			{
				for(int j=0;j<width*gridResolution;j++)
				{
					for(int k=0;k<height*gridResolution;k++)
					{
						p1 = new point(i/gridResolution,j/gridResolution,k/gridResolution);
						temp = interpolatePoint(p1);
						p1->getProperties()->setTemp(temp);
						grid[i][j][k] = *p1;
					}
				}
			}
		}
		double interpolatePoint(point *p)
		{
			double x = p->getX();
			double y = p->getY();
			double z = p->getZ();
			double loc[] = {x,y,z};
			physics *p2 = getClosest(loc);
			//find volume intersect
			double ave=0;
			for(int i=0;i<4;i++)
			{
				if(&p2[i] != NULL)
				{
					x++;
					ave+=p2[i].getTemp();
				}
			}
			return ave=ave/x;
		}
		point getGridPoint(int x, int y, int z)
		{
			return grid[x][y][z];
		}
		space(double lengthBound, double widthBound, double heightBound=1,double res=1)
		{
			length = lengthBound;
			width = widthBound;
			height = heightBound;
			gridResolution = res;
		}
		~space(){}
};

int main()
{
	std::cout<<"hey"<<std::endl;
	double length = 100;
	double width = 100;
	double height = 5;
	double gridResolution = 1;
	double meshResolution = 0.5;
	double x;
	double y;
	double z;
	point p2;
	int size = 10;
	double temp;
	space *model = new space(length,width,height,gridResolution);
	model->setMesh(meshResolution);
	for(double i=0;i<size;i+=gridResolution)
	{
		for(double j=0;j<size;j+=gridResolution)
		{
			for(double k=0;k<size;k+=gridResolution)
			{
				p2 = model->search(i,j,k);
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
	model->interpolateGrid();
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
		{
			//for(int k=0;k<size;k++)
			//{
				p2 = model->getGridPoint(i,j,0);
				temp = p2.getProperties()->getTemp();
				std::cout<<"("<<p2.getX()<<","<<p2.getY()<<","<<p2.getZ()<<") T:"<<temp<<" ";
			//}
		}
		std::cout<<std::endl;
	}
	/*p2 = model.search(5,20,2);
	physics *r = p2.getProperties();
	temp = r->getTemp();
	x = r->getX();
	y = r->getY();
	z = r->getZ();
	std::cout <<"("<< p2.getX() <<","<<p2.getY()<<","<<p2.getZ()<<"): T-"<<temp<<" P("<<
			x<<","<<y<<","<<z<<")"<<std::endl;*/
	//physics closest[] = getClosest(P);
}

