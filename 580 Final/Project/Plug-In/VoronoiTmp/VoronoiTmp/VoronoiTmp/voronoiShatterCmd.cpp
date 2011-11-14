//=============================================== 
// File: voronoiShatterCmd.cpp
//
// MEL Command: voronoiShatter
//
// Authors: 
//================================================

#include "voronoiShatterCmd.h"
#include "voronoiShatterNode.h"

// Function Sets
//
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MFnSingleIndexedComponent.h>

// Iterators
//
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>

// General Includes
//
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MBoundingBox.h>
#include <maya/MMatrix.h>

#include <maya/MIOStream.h>
#include <sstream>

// Status Checking Macro - MCheckStatus (Debugging tool)
//
#define MCheckStatus(status,message)	\
	if( MS::kSuccess != status ) {		\
		cerr << message << "\n";		\
		return status;					\
	}

//	Description:
//		voronoiShatterCmd constructor
VoronoiShatterCmd::VoronoiShatterCmd(void){
}

//	Description:
//		voronoiShatterCmd descructor
VoronoiShatterCmd::~VoronoiShatterCmd(void){
}

//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
void* VoronoiShatterCmd::creator(){
	return new VoronoiShatterCmd();
}

//	Description:
//		this method tells Maya this command is undoable.  It is added to the 
//		undo queue if it is.
//
//	Return Value:
//		true if this command is undoable.
bool VoronoiShatterCmd::isUndoable() const{
	return true;
}

//	Description:
//		implements the MEL splitUV command.
//
//	Arguments:
//		args - the argument list that was passes to the command from MEL
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement.
MStatus VoronoiShatterCmd::doIt( const MArgList& )
{
	MGlobal::displayInfo( "Hello Voronoi!\n" ); 
	return MS::kSuccess; 
	//****************************************************************************************************************
	MStatus status;

	// Parse the selection list for objects with selected UV components.
	// To simplify things, we only take the first object that we find with
	// selected UVs and operate on that object alone.
	//
	// All other objects are ignored and return warning messages indicating
	// this limitation.
	//
	MSelectionList selList;
	MGlobal::getActiveSelectionList( selList );
	MItSelectionList selListIter( selList );
	selListIter.setFilter( MFn::kMesh );

	MFnDagNode nodeFn;
	MFnMesh meshFn;

	MDagPath dagPath;
	MBoundingBox bbx;
	MObject transformMx;
	/*MObject component;
	selListIter.getDagPath( dagPath, component );
	
	dagPath.extendToShape();
	setMeshNode( dagPath );*/

	for( ; !selListIter.isDone(); selListIter.next() ){
		selListIter.getDagPath(dagPath);
		nodeFn.setObject(dagPath);
		bbx = nodeFn.boundingBox();

		transformMx = nodeFn.parent(0);
		nodeFn.setObject(transformMx);
		MMatrix mx = nodeFn.transformationMatrix();

		MPoint max,min;
		max = bbx.max();
		min = bbx.min();

		max *= mx;
		min *= mx;

		Tetrahedron tetra = voronoiShatter.initializeBigTetra(min,max);

		MString output;
		output = MString("Max:") + max.x + "," + max.y + "," + max.z +"," + max.w;
		output += MString("Min:") + min.x + "," + min.y + "," + min.z + "," + min.w;
		// create bounding box
		/*MString v[8];
		v[0]= MString("spaceLocator -a -p ") + max.x + " " + max.y + " " + max.z + ";";
		v[1]= MString("spaceLocator -a -p ") + min.x + " " + max.y + " " + max.z + ";";
		v[2]= MString("spaceLocator -a -p ") + min.x + " " + max.y + " " + min.z + ";";
		v[3]= MString("spaceLocator -a -p ") + max.x + " " + max.y + " " + min.z + ";";
		v[4]= MString("spaceLocator -a -p ") + max.x + " " + min.y + " " + max.z + ";";
		v[5]= MString("spaceLocator -a -p ") + min.x + " " + min.y + " " + max.z + ";";
		v[6]= MString("spaceLocator -a -p ") + min.x + " " + min.y + " " + min.z + ";";
		v[7]= MString("spaceLocator -a -p ") + max.x + " " + min.y + " " + min.z + ";";


		for(int i=0;i<8;i++){
			status = fDGModifier.commandToExecute(v[i]);
		}*/

		MString v[4];
		v[0]= MString("spaceLocator -a -p ") + tetra.v1.point.x + " " + tetra.v1.point.y + " " + tetra.v1.point.z + ";";
		v[1]= MString("spaceLocator -a -p ") + tetra.v2.point.x + " " + tetra.v2.point.y + " " + tetra.v2.point.z + ";";
		v[2]= MString("spaceLocator -a -p ") + tetra.v3.point.x + " " + tetra.v3.point.y + " " + tetra.v3.point.z + ";";
		v[3]= MString("spaceLocator -a -p ") + tetra.v4.point.x + " " + tetra.v4.point.y + " " + tetra.v4.point.z + ";";

		for(int i=0;i<4;i++){
			status = fDGModifier.commandToExecute(v[i]);
		}

		fDGModifier.doIt();

		MGlobal::displayInfo(output);
			//+", Vertex count:"+ meshFn.numVertices() );
	}

	return MS::kSuccess;
	//************************************************************************************************************/
}

//	Description:
//		Implements redo for the MEL splitUV command. 
//
//		This method is called when the user has undone a command of this type
//		and then redoes it.  No arguments are passed in as all of the necessary
//		information is cached by the doIt method.
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
MStatus VoronoiShatterCmd::redoIt(){
	MStatus status;

	// Process the polyModifierCmd
	//
	status = redoModifyPoly();

	if( status == MS::kSuccess )
	{
		setResult( "voronoiShatter command succeeded!" );
	}
	else
	{
		displayError( "voronoiShatter command failed!" );
	}

	return status;
}

//	Description:
//		implements undo for the MEL splitUV command.  
//
//		This method is called to undo a previous command of this type.  The 
//		system should be returned to the exact state that it was it previous 
//		to this command being executed.  That includes the selection state.
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
MStatus VoronoiShatterCmd::undoIt(){
	MStatus status;

	status = undoModifyPoly();

	if( status == MS::kSuccess )
	{
		setResult( "voronoiShatter undo succeeded!" );
	}
	else
	{
		setResult( "voronoiShatter undo failed!" );
	}
    
	return status;
}

//	Description:
//		By overriding the polyModifierCmd::initModifierNode() method,
//		we can insert our own modifierNode initialization code.
//	Arguments:
//		modifierNode - modifier node to initialize
//	Return Value:
//		MS::kSuccess - initialize succeeded
//		MS::kFailure - initialize failed
MStatus VoronoiShatterCmd::initModifierNode( MObject modifierNode ){
	MStatus status;

	status = MS::kSuccess;

	return status;
}

//	Description:
//		Do the modification directly without adding new dependence node
//	Arguments:
//		mesh - mesh which we will play with
//	Return Value:
//		MS::kSuccess - initialize succeeded
//		MS::kFailure - initialize failed
MStatus VoronoiShatterCmd::directModifier( MObject mesh ){
	MStatus status;

	voronoiShatterFactory.setMesh( mesh );
	
	// Perform the voronoiShatter
	status = voronoiShatterFactory.doIt();

	return status;
}