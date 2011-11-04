//=========================================================
//
// File: voronoiShatterNode.h
//
// Dependency Graph Node: voronoiShatterNode
//
// Authors: 
//===========================================================

#ifndef _voronoiShatterNode
#define _voronoiShatterNode

#include "polymodifiernode.h"
#include "voronoiShatterFty.h"
// Maya Includes
//
#include <maya/MTypeId.h>

class VoronoiShatterNode : public polyModifierNode
{
public:
	         VoronoiShatterNode(void);
	virtual ~VoronoiShatterNode(void);
	
	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );

	static  void*		creator();
	static  MStatus		initialize();

public:

	// The typeid is a unique 32bit indentifier that describes this node.
	// It is used to save and retrieve nodes of this type from the binary
	// file format.  If it is not unique, it will cause file IO problems.
	//
	static	MTypeId		id;

	VoronoiShatterFty	voronoiShatterFactory;
};

#endif