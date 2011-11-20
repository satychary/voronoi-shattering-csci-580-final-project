//======================================================= 
// File: vsGeneratePoints.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"
#include <math.h>
#include <time.h>

//	Description:
//
//	Arguments:
//
//	Return Value;
MPoint* VoronoiShatter::generatePoints(int total){
	MPoint* arrayMP;
	arrayMP =(MPoint*)malloc(sizeof(MPoint)*total); 
	MPoint min =boundingBox.min();
	MPoint max =boundingBox.max();

	min *= tMatrix;
	max *= tMatrix;

	MPoint center;
	center.x=(min.x+max.x)/2;
	center.y=(min.y+max.y)/2;
	center.z=(min.z+max.z)/2;
	center.z=1.0;
	double length= sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2))/2;
	int condition=1;
	for(int i=0; i<total;)
	{
		srand ( time(NULL) );
		double x = pow(-1.0,rand())*rand();
		double y = pow(-1.0,rand())*rand();
		double z = pow(-1.0,rand())*rand();

		double A = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
		arrayMP[i].x=x/A*(rand()%(int)length)+center.x;
		arrayMP[i].y=y/A*(rand()%(int)length)+center.y;
		arrayMP[i].z=z/A*(rand()%(int)length)+center.z;
		arrayMP[i].w=1.0;
		if(arrayMP[i].x>max.x || arrayMP[i].x<min.x ||arrayMP[i].y>max.y || arrayMP[i].y<min.y ||arrayMP[i].z>max.z || arrayMP[i].z<min.z)
		{}
		else
		{
			i++;
		}
	}
	return arrayMP;
}