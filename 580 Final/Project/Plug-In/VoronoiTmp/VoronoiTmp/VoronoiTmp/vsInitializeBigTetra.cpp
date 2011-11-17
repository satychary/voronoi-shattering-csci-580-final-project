//======================================================= 
// File: vsInitializeBigTetra.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//		initailuze a big tetra
//	Arguments:
//		min -- left bottom back point of boundingbox
//		max -- right top front point of boundingbox
//	Return Value;
Tetrahedron VoronoiShatter::initializeBigTetra(MPoint min, MPoint max){
	
	MPoint top, left, right, front;
	Vertex topV, leftV, rightV, frontV;
	Tetrahedron tetra;
	min.z -=.5f;
	// top
	top.x = (min.x + max.x)/2;
	top.y = 2*max.y - min.y;
	top.z = min.z;
	top.w = 1;

	// left
	left.x = (min.x+max.x)/2 - 3*(max.x-min.x);
	left.y = min.y;
	left.z = min.z;
	left.w = 1;

	// right
	right.x = (min.x+max.x)/2 + 3*(max.x-min.x);
	right.y = min.y;
	right.z = min.z;
	right.w = 1;
	
	// front
	front.x = (min.x+max.x)/2;
	front.y = min.y;
	front.z = min.z + 3*(max.z-min.z);
	front.w = 1;

	// construct tetra
	topV.point = top;
	leftV.point = left;
	rightV.point = right;
	frontV.point = front;

	tetra.v1 = topV ;
	tetra.v2 = leftV;
	tetra.v3 = rightV;
	tetra.v4 = frontV;
	
	// add to pool
	addTetra(tetra);

	return tetra;
}