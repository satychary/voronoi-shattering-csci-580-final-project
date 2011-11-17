//======================================================= 
// File: vsGenerateOnePoint.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"
#include <math.h>
//	Description:
//
//	Arguments:
//
//	Return Value;
void VoronoiShatter::generateOnePoint(MPoint &newpoint){
	MPoint min =boundingBox.min();
	MPoint max =boundingBox.max();
	MPoint center;
	center.x=(min.x+max.x)/2;
	center.y=(min.y+max.y)/2;
	center.z=(min.z+max.z)/2;
	center.z=1.0;
	double length= sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2))/2;
	int condition=1;
	while(condition)
	{
		double x = pow(-1.0,rand())*rand();
		double y = pow(-1.0,rand())*rand();
		double z = pow(-1.0,rand())*rand();
		double A = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		newpoint.x=x/A*length+center.x;
		newpoint.y=y/A*length+center.y;
		newpoint.z=z/A*length+center.z;
		newpoint.w=1.0;
		if(newpoint.x>max.x || newpoint.x<min.x ||newpoint.y>max.y || newpoint.y<min.y ||newpoint.z>max.z || newpoint.z<min.z)
		{
			condition=1;
		}
		else
		{
			condition=0;
		}
	}
	return;
}