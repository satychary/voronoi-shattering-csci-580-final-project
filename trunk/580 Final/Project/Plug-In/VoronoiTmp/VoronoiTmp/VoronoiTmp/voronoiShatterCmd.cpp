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
#include <maya/MFnSet.h>

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
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MIOStream.h>
#include <sstream>
#include <map>

// Status Checking Macro - MCheckStatus (Debugging tool)
//
#define MCheckStatus(status,message)	\
	if( MS::kSuccess != status ) {		\
		cerr << message << "\n";		\
		return status;					\
	}

const char *numberFlag = "-n", *numberLongFlag = "-number";
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
//		add arguments 
//	Return Value:
//		
MSyntax VoronoiShatterCmd::newSyntax(){
	MSyntax syntax;
    syntax.addFlag( numberFlag, numberLongFlag, MSyntax::kLong );
//    syntax.addFlag( radiusFlag, radiusLongFlag, MSyntax::kDouble );

	return syntax;
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
MStatus VoronoiShatterCmd::doIt( const MArgList &args )
{

	//	MGlobal::displayInfo( "Hello Voronoi!\n" ); 
    //	return MS::kSuccess; 
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
	MObject dependNode;
	MObject newMesh;
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

		voronoiShatter.setTransformMatrix(mx);
		voronoiShatter.setBoundingBox(bbx);
		Tetrahedron tetra = voronoiShatter.initializeBigTetra();

		MString output;
	//	output = MString("Max:") + max.x + "," + max.y + "," + max.z +"," + max.w;
	//	output += MString("Min:") + min.x + "," + min.y + "," + min.z + "," + min.w;
		
		// display bounding box
        /**************************************************************************** 
        MString v[8];
		v[0]= MString("spaceLocator -p ") + max.x + " " + max.y + " " + max.z + ";";
		v[1]= MString("spaceLocator -p ") + max.x + " " + max.y + " " + min.z + ";";
		v[2]= MString("spaceLocator -p ") + max.x + " " + min.y + " " + max.z + ";";
		v[3]= MString("spaceLocator -p ") + max.x + " " + min.y + " " + min.z + ";";

		v[4]= MString("spaceLocator -p ") + min.x + " " + max.y + " " + max.z + ";";
		v[5]= MString("spaceLocator -p ") + min.x + " " + max.y + " " + min.z + ";";
		v[6]= MString("spaceLocator -p ") + min.x + " " + min.y + " " + max.z + ";";
		v[7]= MString("spaceLocator -p ") + min.x + " " + min.y + " " + min.z + ";";
		for(int i=0;i<8;i++){
			status = fDGModifier.commandToExecute(v[i]);
		}
        //*****************************************************************************/
		
		// create big tetra mesh
		//******************************************************************************		
		MPoint vertex[4] ={tetra.v1.point,tetra.v2.point,tetra.v3.point,tetra.v4.point};
		MPointArray vertexArray(vertex, 4);
		int counts[4] = {3,3,3,3};
		MIntArray polyCounts(counts,4);
		int connects[12] = {0,1,3,0,2,3,0,1,2,1,2,3};
		MIntArray polyConnects(connects,12);
		selListIter.getDependNode(dependNode);
		newMesh = meshFn.create(4,4,vertexArray,polyCounts,polyConnects);
		//********************************************************************************/
		
		// assign shader
		//********************************************************************************	
		MObjectArray shaders, comps;
		MFnSet setFn;
		meshFn.setObject(dagPath);
		meshFn.getConnectedSetsAndMembers(dagPath.instanceNumber(),shaders,comps,true);
		setFn.setObject(shaders[0]);
		setFn.addMember(newMesh);
		//********************************************************************************/
		
		// generate points
		//*******************************************************************************************************
		int numPoints = 3;
		MArgDatabase argData( syntax(), args );
		if( argData.isFlagSet( numberFlag ) )
			argData.getFlagArgument( numberFlag, 0, numPoints );
		MPoint* points;
		MString locatorCmd;
		points = voronoiShatter.generatePoints(numPoints);
		for(int i=0;i<numPoints;i++){
			locatorCmd = MString("spaceLocator -a -p ") + points[i].x + " " + points[i].y + " " + points[i].z + ";";
			status = fDGModifier.commandToExecute(locatorCmd);
		}
		/************************************************************************************************************/

		// Insert on point
		/****************************************************
		MPoint point(0,0,0,1);
		voronoiShatter.insertOnePoint(point);
		//**************************************************/

		// add polygon
		/**********************************************************************************
		meshFn.setObject(newMesh);
		TetraMap pool = voronoiShatter.getPool();
		TetraMapItr itr= pool.begin();
		for(;itr!=pool.end();itr++){
			Tetrahedron tetra = itr->second;
			MPoint tri1[3]={tetra.v1.point,tetra.v2.point,tetra.v3.point};
			MPointArray vertexArr1(tri1, 3);

			MPoint tri2[3]={tetra.v1.point,tetra.v2.point,tetra.v4.point};
			MPointArray vertexArr2(tri2, 3);

			MPoint tri3[3]={tetra.v1.point,tetra.v3.point,tetra.v4.point};
			MPointArray vertexArr3(tri3, 3);

			MPoint tri4[3]={tetra.v2.point,tetra.v3.point,tetra.v4.point};
			MPointArray vertexArr4(tri4, 3);
			
			meshFn.addPolygon(vertexArr1);
			meshFn.addPolygon(vertexArr2);
			meshFn.addPolygon(vertexArr3);
			meshFn.addPolygon(vertexArr4);
		}
		//********************************************************************************/

		// test ORIENT
		/**********************************************************************************	
		MPoint p(0,0,0,1);
		double rst;
		rst = voronoiShatter.orient(tetra.v1,tetra.v2,tetra.v3,p);
		if(rst<0)
			output = "under!";
		else if(rst>0)
			output = "above!";
		else
			output = "on!"; 
		//***********************************************************************************/
		
		// find point
		/*************************************************************************************		
		MPoint p(0,10,0,1);
		MString cmd = MString("spaceLocator -p ") + p.x + " " + p.y + " " + p.z + ";";
		fDGModifier.commandToExecute(cmd);

		bool rst = voronoiShatter.findPoint(p, tetra);
		if(rst){
			output = MString("Found! ")+tetra.key;
		}
		else
			output = MString("Not Found!");
		//************************************************************************************/

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