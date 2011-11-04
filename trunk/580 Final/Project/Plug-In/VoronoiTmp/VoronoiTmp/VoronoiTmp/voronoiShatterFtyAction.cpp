//========================================================= 
// File: voronoiShatterFtyAction.cpp
//
// Node Factory: voronoiShatterFty
//
// Author:
//=========================================================

#include "voronoiShatterFty.h"

// General Includes
//
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>

// Function Sets
//
#include <maya/MFnMesh.h>
// Iterators
//
#include <maya/MItMeshPolygon.h>

//	Description:
//		Performs the actual voronoiShatter operation on the given object
MStatus VoronoiShatterFty::doIt(){
	MStatus status = MS::kSuccess;

	MFnMesh meshFn( fMesh );
	MItMeshPolygon polyIter( fMesh );

	meshCount = polyIter.count();

	return status;
}