/*
 * Physics.cpp
 *	will house various physical properties
 *  Created on: Jan 10, 2016
 *      Author: Kaliel
 */
#include <numeric>

class physics
{
	private:
		double temperature;
		//conservation of mass - massIn = massOut
		//conservation of momentum
		//double inVector[6][3];
		double outVector[3] = {0,0,0};
	public:
		double getTemp(){return temperature;}
		void setTemp(double input){temperature = input;}
		void setVector(double x, double y, double z)
		{
			outVector[0] += x;
			outVector[1] += y;
			outVector[2] += z;
		}
		double* getVector()
		{
			return outVector;
		}
		double getX(){return outVector[0];}
		double getY(){return outVector[1];}
		double getZ(){return outVector[2];}
		physics(){}
		~physics(){}
		//at this point, put in and solve for the the Navier-Stokes equations
		/*u, v, w, p wrt x,y,z,t for each point in the grid,
		 *focusing on where derivatives in these are high
		*/
};

