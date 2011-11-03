//======================================================= 
// File: voronoiShatter.h
//
// Authors: 
//=========================================================

#ifndef _voronoiShatter
#define _voronoiShatter

#include "maya/MPoint.h"

struct vertex{
	MPoint point;
	tetrahedron incidentTetra; // optional
};

struct tetrahedron{
	vertex *v1;                // vertex
	vertex *v2;
	vertex *v3;
	vertex *v4;
	tetrahedron *neighbour1;   // neighbour
	tetrahedron *neighbour2;
	tetrahedron *neighbour3;
	tetrahedron *neighbour4;
};

class voronoiShatter
{
public:
	voronoiShatter(void);
	~voronoiShatter(void);

	//  Public Method
	//
	//  To team members: 
	//		Change the return value and the arguments as u need!
	void initializeBigTetra(); // CZ

	void orient();             // R
	void inSphere();

	void findPoint();          // N, using WALK algorithm

	void insertOnePoint();     // B
	void flip();       

	void getVDFormDT();        // CZ

	// Mian action
	void perform();


};

#endif