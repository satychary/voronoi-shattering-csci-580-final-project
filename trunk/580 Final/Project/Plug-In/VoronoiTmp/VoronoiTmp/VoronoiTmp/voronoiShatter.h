//======================================================= 
// File: voronoiShatter.h
//
// Authors: 
//=========================================================

#ifndef _voronoiShatter
#define _voronoiShatter

#include "maya/MPoint.h"
struct Tetrahedron;

struct Vertex{
	MPoint point;
    Tetrahedron *incidentTetra; // optional
};

struct Tetrahedron{
	Vertex *v1;                // vertex
	Vertex *v2;
	Vertex *v3;
	Vertex *v4;
	Tetrahedron *neighbour1;   // neighbour
	Tetrahedron *neighbour2;
	Tetrahedron *neighbour3;
	Tetrahedron *neighbour4;
};

class VoronoiShatter
{
public:
	VoronoiShatter(void);
	~VoronoiShatter(void);

	//  Public Method
	//
	//  To team members: 
	//		Change the return value and the arguments as u need!
	void initializeBigTetra(); // CZ

	double orient(Vertex *v1, Vertex *v2, Vertex *v3, MPoint p);             // R
	double inSphere(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *v4, MPoint p);

	void findPoint();          // N, using WALK algorithm

	void insertOnePoint();     // B
	void flip();       

	void getVDFormDT();        // CZ

	// Mian action
	void perform();


};

#endif