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
#include <maya/MItMeshEdge.h>

// General Includes
//
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MBoundingBox.h>
#include <maya/MMatrix.h>
#include <maya/MFloatPointArray.h>
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
const char *sectionFlag = "-s", *sectionLongFlag = "-section";
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
	syntax.addFlag( sectionFlag, sectionLongFlag, MSyntax::kLong );
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
		// set mesh
		//voronoiShatter.setMesh(dagPath);
		meshFn.setObject(dagPath);
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
		/******************************************************************************		
		newMesh = createTetraMesh(tetra, meshFn);
		//********************************************************************************/
		
		// assign shader
		/********************************************************************************	
		MObjectArray shaders, comps;
		MFnSet setFn;
		meshFn.setObject(dagPath);
		meshFn.getConnectedSetsAndMembers(dagPath.instanceNumber(),shaders,comps,true);
		setFn.setObject(shaders[0]);
		setFn.addMember(newMesh);
		//********************************************************************************/
		
		// generate points
		/*******************************************************************************************************
		int numPoints = 3;
		int numSections = 1;
		MArgDatabase argData( syntax(), args );
		if( argData.isFlagSet( numberFlag ) )
			argData.getFlagArgument( numberFlag, 0, numPoints );
		if( argData.isFlagSet( sectionFlag ) )
			argData.getFlagArgument( sectionFlag, 0, numSections );

		MPoint* points;
		MString locatorCmd;
		points = voronoiShatter.generatePoints(numPoints,numSections);
		for(int i=0;i<numPoints;i++){
			locatorCmd = MString("spaceLocator -a -p ") + points[i].x + " " + points[i].y + " " + points[i].z + ";";
			status = fDGModifier.commandToExecute(locatorCmd);
		}
		/************************************************************************************************************/

		// Insert on point
		/****************************************************
		MPoint point(0,0,0,1);
		voronoiShatter.insertOnePoint(point);
		MString cmd = MString("spaceLocator -p ") + point.x + " " + point.y + " " + point.z + ";";
		fDGModifier.commandToExecute(cmd);
		point.y = 1;
		point.x = 2;
		voronoiShatter.insertOnePoint(point);
	    cmd = MString("spaceLocator -p ") + point.x + " " + point.y + " " + point.z + ";";
		fDGModifier.commandToExecute(cmd);
		//**************************************************/

		// add polygon
		/**********************************************************************************
		meshFn.setObject(newMesh);
		TetraMap pool = voronoiShatter.getPool();
		TetraMapItr itr= pool.begin();
		for(;itr!=pool.end();itr++){
			Tetrahedron tetra = itr->second;
			newMesh = createTetraMesh(tetra,meshFn);
			assignShader(newMesh, meshFn,dagPath);
			//MPoint tri1[3]={tetra.v1.point,tetra.v2.point,tetra.v3.point};
			//MPointArray vertexArr1(tri1, 3);

			//MPoint tri2[3]={tetra.v1.point,tetra.v2.point,tetra.v4.point};
			//MPointArray vertexArr2(tri2, 3);

			//MPoint tri3[3]={tetra.v1.point,tetra.v3.point,tetra.v4.point};
			//MPointArray vertexArr3(tri3, 3);

			//MPoint tri4[3]={tetra.v2.point,tetra.v3.point,tetra.v4.point};
			//MPointArray vertexArr4(tri4, 3);
			
			//meshFn.addPolygon(vertexArr1);
			//meshFn.addPolygon(vertexArr2);
			//meshFn.addPolygon(vertexArr3);
			//meshFn.addPolygon(vertexArr4);
		}
		//meshFn.updateSurface();
	//	output = MString(" neighbors:") + pool.begin()->second.neighbour1+", "
		//	+ pool.begin()->second.neighbour2 + ", "
		//	+ pool.begin()->second.neighbour3 + ","
		//  + pool.begin()->second.neighbour4 + " key"
		//	+ pool.begin()->second.key;
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
		MPoint p(-1,-0.5,0,1);
		MString cmd = MString("spaceLocator -p ") + p.x + " " + p.y + " " + p.z + ";";
		fDGModifier.commandToExecute(cmd);

		bool rst = voronoiShatter.findPoint(p, tetra);
		if(rst){
			output = MString("Found! ")+tetra.key;
		}
		else
			output = MString("Not Found!");
		//************************************************************************************/


		// test fnMeshEdge
		/*MItMeshEdge edgeIt(dagPath);
		for(;!edgeIt.isDone();edgeIt.next()){
			output += MString("id: ") + edgeIt.index();
		}
		for(int i=0; i<4;edgeIt.next()){
			MPoint p = edgeIt.point(1,MSpace::kWorld);
			p.x = 0;
			edgeIt.setPoint(p,1,MSpace::kWorld);

			i++;
		}
		Vertex v1,v2,v3;

		v1.point.x = 0;
		v1.point.y = 0;
		v1.point.z = 1;

		v2.point.x = 0;
		v2.point.y = 1;
		v2.point.z = 0;

		v3.point.x = 0;
		v3.point.y = 0;
		v3.point.z = -1;

		int orient;
		MPoint p = edgeIt.point(0, MSpace::kWorld);
		orient = voronoiShatter.orient(v1,v2,v3,p);
		if(orient<0){
			output += MString("point 0: under!");
		}
		else{
			output += MString("point 0: above!");
		}
		//output += MString("Position 0: ")+p.x+" " + p.y +" "+p.z;
		p = edgeIt.point(1, MSpace::kWorld);
		orient = voronoiShatter.orient(v1,v2,v3,p);
		if(orient<0){
			output += MString("point 1: under!");
		}
		else{
			output += MString("point 1: above!");
		}
		//output += MString("Position 1: ")+p.x+" " + p.y +" "+p.z;
		//p.x = 0;
		//edgeIt.setPoint(p,1,MSpace::kWorld);
		//meshFn.deleteEdge(0, &fDGModifier);
		//meshFn.deleteEdge(2, &fDGModifier);

		int place[5] = {MFnMesh::kOnEdge, MFnMesh::kOnEdge,MFnMesh::kOnEdge, MFnMesh::kOnEdge, MFnMesh::kOnEdge};
		MIntArray placements(place,5);

		int list[5] = {0,1,2,3,0};
		MIntArray edgeList(list,5);
		
		float factor[5] = {.5,.5,.5,.5,.5};
		MFloatArray edgeFactors(factor,5);

		MFloatPointArray internalpoints;

		meshFn.split(placements,edgeList,edgeFactors,internalpoints);
		*/

		Plane plane;
		plane.pa.x = 1;
		plane.pa.y = -3;
		plane.pb.z = 3;

		plane.pb.x = 0;
		plane.pb.y = 3;
		plane.pb.z = 0;

		plane.pc.x = -1;
		plane.pc.y = -3;
		plane.pc.z = -3;

		voronoiShatter.splitMesh(dagPath,plane);
		meshFn.updateSurface();
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

//	Description:
//		create a tetra mesh
//	Arguments:
//		tetra  -- the tetra which the mesh based on
//      meshFn -- MFnMesh
//	Return Value:
//		the result mesh
MObject VoronoiShatterCmd::createTetraMesh(Tetrahedron tetra, MFnMesh meshFn)
{
	MObject newMesh;
	
	MPoint vertex[4] ={tetra.v1.point,tetra.v2.point,tetra.v3.point,tetra.v4.point};
	MPointArray vertexArray(vertex, 4);
	int counts[4] = {3,3,3,3};
	MIntArray polyCounts(counts,4);
	int connects[12] = {0,1,3,0,2,3,0,1,2,1,2,3};
	MIntArray polyConnects(connects,12);
	newMesh = meshFn.create(4,4,vertexArray,polyCounts,polyConnects);
	
	return newMesh;

}

//	Description:
//		add a mesh to a shadering group
//	Arguments:
//		mesh     -- mesh to be assign
//		meshFn   -- MFnMesh function set
//		MDagPath -- MDagPath
//	Return Value:
void VoronoiShatterCmd::assignShader(MObject mesh, MFnMesh meshFn, MDagPath dagPath)
{
	MObjectArray shaders, comps;
	MFnSet setFn;
	meshFn.setObject(dagPath);
	meshFn.getConnectedSetsAndMembers(dagPath.instanceNumber(),shaders,comps,true);
	setFn.setObject(shaders[0]);
	setFn.addMember(mesh);
}