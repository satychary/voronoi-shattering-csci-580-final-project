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
MPoint* VoronoiShatter::generatePoints(int total, int section){

	//int section=5;
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

			arrayMP[counter].x = (x/A)*((rand()%(int)sect_length)+sect_length*(k-1))+center.x;
			arrayMP[counter].y = (y/A)*((rand()%(int)sect_length)+sect_length*(k-1))+center.y;
			arrayMP[counter].z = (z/A)*((rand()%(int)sect_length)+sect_length*(k-1))+center.z;
			arrayMP[counter].w = 1.0;
			if(arrayMP[counter].x>max.x || arrayMP[counter].x<min.x ||arrayMP[counter].y>max.y || arrayMP[counter].y<min.y ||arrayMP[counter].z>max.z || arrayMP[counter].z<min.z)
			{}
			else
			{
				counter++;
				i++;
			}
		}
	}
	return arrayMP;
}