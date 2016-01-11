/*
 * Points.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: Kaliel
 */
#import "Physics.cpp"

class point
{
	private:
		double location[3];
		physics properties;
	public:
		void setX(double input)
		{
			location[0] = input;
		}
		void setY(double input)
		{
			location[1] = input;
		}
		void setZ(double input)
		{
			location[2] = input;
		}
		double getX()
		{
			return location[0];
		}
		double getY()
		{
			return location[1];
		}
		double getZ()
		{
			return location[2];
		}
		void setVector(double x, double y, double z)
		{
			properties.setVector(x,y,z);
		}
		double* getVector()
		{
			return properties.getVector();
		}
		double* getLocation()
		{
			return location;
		}
		physics* getProperties()
		{
			return &properties;
		}
		void setProperties(physics input)
		{
			properties = input;
		}
		/*for each point, find the closest tree nodes, get
		 * the average of their corresponding properties and
		 * assign that to the property. Later, estimate a plane
		 * or line for the point
		 */
		point(){setX(0);setY(0);setZ(0);}
		point(int x, int y, int z = 0)
		{
			setX(x);
			setY(y);
			setZ(z);
		}
};


