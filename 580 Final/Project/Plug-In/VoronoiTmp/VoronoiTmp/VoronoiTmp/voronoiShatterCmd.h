//========================================= 
// File: voronoiShatterCmd.h
//
// MEL Command: voronoiShatter
//
// Authors: 
//===========================================

#ifndef _voronoiShatterCmd
#define _voronoiShatterCmd

#include "polymodifiercmd.h"
#include "voronoiShatterFty.h"
#include "voronoiShatter.h"

// Function Sets
//
#include <maya/MFnComponentListData.h>

// Forward Class Declarations
//
class MArgList;
class VoronoiShatterCmd : public polyModifierCmd
{
public:
	VoronoiShatterCmd(void);
	~VoronoiShatterCmd(void);
	static		void* creator();

	bool		isUndoable() const;

	MStatus		doIt( const MArgList& );
	MStatus		redoIt();
	MStatus		undoIt();

	// polyModifierCmd Methods //

	MStatus		initModifierNode( MObject modifierNode );
	MStatus		directModifier( MObject mesh );

private:

	// Private Methods //

	// Private Data //
	VoronoiShatter voronoiShatter;
	// voronoiShatter Factory
	VoronoiShatterFty			voronoiShatterFactory;
};

#endif