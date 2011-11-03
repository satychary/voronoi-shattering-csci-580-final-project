//======================================================= 
// File: voronoiShatterFty.h
//
// Node Factory: voronoiShatterFty
//
// Authors: 
//=========================================================

#ifndef _voronoiShatterFty
#define _voronoiShatterFty

#include "polymodifierfty.h"

// Maya Include
//
#include <maya/MObject.h>

class voronoiShatterFty : public polyModifierFty
{
public:
				voronoiShatterFty();
	virtual		~voronoiShatterFty();

	void		 setMesh( MObject mesh );
	static int	 getMeshCount();

	// polyModifierFty inherited methods
	//
	MStatus		doIt();

	static int  meshCount;

private:
	// Mesh Node - The node contain the mesh which we will play with
	//
	MObject		fMesh;
};

#endif