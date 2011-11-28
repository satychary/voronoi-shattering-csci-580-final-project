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
#include <maya/MFnMesh.h>

#include "Globals.h"

// Forward Class Declarations
//
class MArgList;
class VoronoiShatterCmd : public polyModifierCmd
{
public:
	VoronoiShatterCmd(void);
	~VoronoiShatterCmd(void);
	static		void* creator();
	static MSyntax newSyntax();

	bool		isUndoable() const;

	MStatus		doIt( const MArgList &args);
	MStatus		redoIt();
	MStatus		undoIt();

	// polyModifierCmd Methods //

	MStatus		initModifierNode( MObject modifierNode );
	MStatus		directModifier( MObject mesh );

	MObject     createTetraMesh(Tetrahedron tetra, MFnMesh &meshFn);
	void        assignShader(MObject mesh, MFnMesh meshFn,MDagPath dagPath);

private:

	// Private Methods //

	// Private Data //
	VoronoiShatter voronoiShatter;
	// voronoiShatter Factory
	VoronoiShatterFty			voronoiShatterFactory;
};

#endif