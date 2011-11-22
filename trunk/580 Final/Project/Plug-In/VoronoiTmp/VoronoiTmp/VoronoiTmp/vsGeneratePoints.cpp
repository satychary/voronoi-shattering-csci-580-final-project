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
VertexSet VoronoiShatter::generatePoints(int total, int section){

	VertexSet setV;
	Vertex temp_v;
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
	center.w=1.0;

	double length= sqrt(pow((max.x-min.x),2)+pow((max.y-min.y),2)+pow((max.z-min.z),2))/2;
	double sect_length= length/section;
	double sum=0;
	int section_total;
	int counter=0;
	srand ( time(NULL) );

	for(int j=1;j<=section;j++)//sum = sigma (1/x^2)
	{
		sum += (1/pow((double)j,2));
	}

	for(int k=1;k<=section;k++)
	{
		if(k==section)
		{
			section_total=total-counter-1;//last section
		}
		else
		{
			section_total=(int)(((1/pow((double)k,2))/sum)*total);//how many points does each section has.
		}

		for(int i=0; i<section_total;)
		{
			//srand ( time(NULL) );
			double x = pow(-1.0,rand())*rand();
			double y = pow(-1.0,rand())*rand();
			double z = pow(-1.0,rand())*rand();
			double A = sqrt(pow(x,2)+pow(y,2)+pow(z,2));

			temp_v.point.x = (x/A)*((rand()%((int)sect_length)+1)+sect_length*(k-1))+center.x;
			temp_v.point.y = (y/A)*((rand()%((int)sect_length)+1)+sect_length*(k-1))+center.y;
			temp_v.point.z = (z/A)*((rand()%((int)sect_length)+1)+sect_length*(k-1))+center.z;
			temp_v.point.w = 1.0;
			if(!(temp_v.point.x>max.x || temp_v.point.x<min.x ||temp_v.point.y>max.y || temp_v.point.y<min.y ||temp_v.point.z>max.z || temp_v.point.z<min.z))
			{
				if(setV.find(temp_v)!= setV.end())
				{
					setV.insert(temp_v);
					counter++;
					i++;
				}
			}
		}
	}
	return setV;
}