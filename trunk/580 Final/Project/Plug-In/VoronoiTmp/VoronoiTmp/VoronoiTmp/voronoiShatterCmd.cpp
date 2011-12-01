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

void VoronoiShatterCmd::displayLocator(MPoint p)
{
	   MString v;
		v = MString("spaceLocator -p ") + p.x + " " + p.y + " " + p.z + ";";
	    fDGModifier.commandToExecute(v);
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
		TetraMap pool = voronoiShatter.getPool();

		MString output;
	//	output = MString("Hello ") + tetra.key;
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
		
		// copy mesh
		/******************************************************************************
		nodeFn.duplicate();
		//*****************************************************************************/

		// create big tetra mesh
		//******************************************************************************		
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
		VertexSet vertex;
		VertexSetItr vItr;
		MString locatorCmd;
		vertex = voronoiShatter.generatePoints(numPoints,numSections);
		vItr = vertex.begin();
		for(;vItr!=vertex.end();vItr++){
			this->displayLocator(vItr->point);
			voronoiShatter.insertOnePoint(vItr->point);
		}
		fDGModifier.doIt();
		/************************************************************************************************************/

		// Insert on point
		//****************************************************
		MPoint point1(0,1,0,1);
		voronoiShatter.insertOnePoint(point1);
		this->displayLocator(point1);
		voronoiShatter.insertOnePoint(point1);

		MPoint point2(0,6,0,1);
		voronoiShatter.insertOnePoint(point2);
		this->displayLocator(point2);
		voronoiShatter.insertOnePoint(point2);
		//voronoiShatter.insertOnePoint(point);
	    //cmd = MString("spaceLocator -p ") + point.x + " " + point.y + " " + point.z + ";";
	//	fDGModifier.commandToExecute(cmd);
		//**************************************************/

		// add polygon
		//**********************************************************************************
		pool = voronoiShatter.getPool();
		TetraMapItr tetraItr= pool.begin();
		for(;tetraItr!=pool.end();tetraItr++){
			Tetrahedron tetra = tetraItr->second;
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
		meshFn.updateSurface();
/*	    itr= pool.begin();
		itr ++;
		itr ++;
		itr ++;
		output = MString(" neighbors:") + itr->second.neighbour1+", "
			+ itr->second.neighbour2 + ", "
			+ itr->second.neighbour3 + ","
		  + itr->second.neighbour4 + " key"
			+ itr->second.key;*/
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
		/************************************************************************************
		MItMeshEdge edgeIt(dagPath);
	//	for(;!edgeIt.isDone();edgeIt.next()){
		//	output += MString("id: ") + edgeIt.index();
		//}
		output  = MString("edge A: ") + edgeIt.point(0).x + ","+edgeIt.point(0).y + ","+edgeIt.point(0).z + ";";
		output += MString("edge B: ") + edgeIt.point(1).x + ","+edgeIt.point(1).y + ","+edgeIt.point(1).z + ";";
		this->displayLocator(edgeIt.point(0));
		this->displayLocator(edgeIt.point(1));

	/*	for(int i=0; i<4;edgeIt.next()){
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

		meshFn.split(placements,edgeList,edgeFactors,internalpoints);*/
		//*******************************************************************************************************/

		// test DT -> VD
        //****************************************************************************************************
		voronoiShatter.getVDFormDT();
		/**/std::vector<int>::iterator polyItr = voronoiShatter.VDpolyIndex.begin();
		for(;polyItr!=voronoiShatter.VDpolyIndex.end();polyItr++){
			int polyID = *polyItr;
			int startFaceId;
			int endFaceId;
			int faceCount;
	
			startFaceId = *polyItr;
			polyItr++;
			endFaceId = *polyItr;
			polyItr--;
			faceCount = endFaceId - startFaceId;

			MPointArray vertexArray;
			MIntArray polyCounts;
			MIntArray polyConnects;
			MPointArray pointArr;
			for(int i=startFaceId;i<endFaceId;i++)
			{
				//MPointArray pointArr;
				voronoiShatter.getPolyFace(i,pointArr);
				polyCounts.append(pointArr.length());
				for(int j=0;j<pointArr.length();j++){
					vertexArray.append(pointArr[j]);
					this->displayLocator(pointArr[j]);
				}
			}
			for(int j=0;j<vertexArray.length();j++){
				polyConnects.append(j);
			}
			MObject newMesh;
			newMesh = meshFn.create(vertexArray.length(),faceCount,vertexArray,polyCounts,polyConnects);
		} 
		std::vector<Vertex>::iterator itr = voronoiShatter.VDvertex.begin();
		for(;itr!=voronoiShatter.VDvertex.end();itr++)
		{
			this->displayLocator(itr->point);
		}
		output = MString("Vertex num: ")+ voronoiShatter.VDvertex.size(); 
		output += MString("Edge num: ")+ voronoiShatter.VDedge.size();
		output += MString("Face num: ")+ voronoiShatter.VDfaceIndex.size();
		output += MString(" Polygon num: ")+ voronoiShatter.VDpolyIndex.size(); 
	//	output += MString("Last 2 Face : ")+ voronoiShatter.VDfaceIndex.at(2);
		output += MString("Face : ")+ voronoiShatter.VDfaceIndex.at(4);
		output += MString(" Polygon : ")+ voronoiShatter.VDpolyIndex.size();

		//****************************************************************************************************/

        // test split
		/***************************************************************************
		//nodeFn.duplicate();
		
		pool = voronoiShatter.getPool();
		TetraMapItr itr = pool.begin();
		for(;itr!=pool.end();itr++){
			// get a copy of the mesh to shatter
			MObject copyMesh;
			copyMesh = nodeFn.duplicate();
			MDagPath newDagPath;
			newDagPath = MDagPath::getAPathTo(copyMesh);
			assignShader(copyMesh, meshFn, dagPath);
			fDGModifier.doIt();
			// shatter it!
			voronoiShatter.split(newDagPath,itr->second);
		}
		
		//***************************************************************************/
		
		// test split mesh
		/******************************************************************************
		pool = voronoiShatter.getPool();
		TetraMapItr itr = pool.begin();
		tetra = itr->second;

			MObject copyMesh;
			copyMesh = nodeFn.duplicate();
			MDagPath newDagPath;
			newDagPath = MDagPath::getAPathTo(copyMesh);
			assignShader(copyMesh, meshFn, dagPath);
			meshFn.setObject(copyMesh);
			fDGModifier.doIt();

			int orient;
			Plane plane;

			// plane 1 
			voronoiShatter.vertexToPlane(tetra.v1,tetra.v2,tetra.v3,plane);
			orient = voronoiShatter.orient(tetra.v1,tetra.v2,tetra.v3,tetra.v4.point);
			voronoiShatter.splitMesh(newDagPath,plane,orient);
			meshFn.updateSurface();
			fDGModifier.doIt();
			// plane 3
			voronoiShatter.vertexToPlane(tetra.v2,tetra.v3,tetra.v4,plane);
			orient = voronoiShatter.orient(tetra.v2,tetra.v3,tetra.v4,tetra.v1.point);
			voronoiShatter.splitMesh(newDagPath,plane,orient);
			meshFn.updateSurface();
			fDGModifier.doIt();
			// plane 2
			voronoiShatter.vertexToPlane(tetra.v1,tetra.v2,tetra.v4,plane);
			orient = voronoiShatter.orient(tetra.v1,tetra.v2,tetra.v4,tetra.v3.point);
			voronoiShatter.splitMesh(newDagPath,plane,orient);
			meshFn.updateSurface();
			fDGModifier.doIt();
			// plane 4
			voronoiShatter.vertexToPlane(tetra.v1,tetra.v3,tetra.v4,plane);
			orient =voronoiShatter.orient(tetra.v1,tetra.v3,tetra.v4,tetra.v2.point);
			voronoiShatter.splitMesh(newDagPath,plane,orient);
			meshFn.updateSurface();
			fDGModifier.doIt();
			MItMeshEdge edgeItr(newDagPath);
			output = MString("Count£º") + edgeItr.count();
		//******************************************************************************/

		// test intersection
		/************************************************************
		pool = voronoiShatter.getPool();
		TetraMapItr itr = pool.begin();
		tetra = itr->second;
		int orient;
		Plane plane;
		voronoiShatter.vertexToPlane(tetra.v1,tetra.v3,tetra.v4,plane);
		Line line;
		line.la = MPoint(-5,-5,0.53);
		line.lb = MPoint(5,-5,3.016);
		this->displayLocator(line.la);
		this->displayLocator(line.lb);
		MPoint p;
		p = voronoiShatter.getLinePlaneIntersection(line, plane);
		output = MString("point: ") + p.x+","+p.y+","+p.z+";";
		this->displayLocator(p);
		//***********************************************************/

// debug
/**********************************
		pool = voronoiShatter.getPool();
		TetraMapItr itr = pool.begin();
		tetra = itr->second;

		MObject copyMesh;
		copyMesh = nodeFn.duplicate();
		MDagPath newDagPath;
		newDagPath = MDagPath::getAPathTo(copyMesh);
		newDagPath.extendToShape();
		assignShader(copyMesh, meshFn, dagPath);
		meshFn.setObject(copyMesh);
		fDGModifier.doIt();

		MPointArray intersectionArr;
		int orient;
		Plane plane;

		// plane 4
		voronoiShatter.vertexToPlane(tetra.v1,tetra.v3,tetra.v4,plane);
		orient = voronoiShatter.orient(tetra.v1,tetra.v3,tetra.v4,tetra.v2.point);
		//voronoiShatter.splitMesh(newDagPath,plane,orient);

		//MFnMesh meshFn(newDagPath);
		MFnMesh meshFn(newDagPath);
		MItMeshEdge edgeItr(newDagPath);
		MItMeshEdge edgeItr4Del(newDagPath);
		MItMeshVertex vertexItr(newDagPath);
		MItMeshVertex vertexItr4Del(newDagPath);

	Line line;
		for(;!edgeItr.isDone();edgeItr.next()){
		
		edgeItr.updateSurface();

		// set a b
		//line.la = edgeIt.point(0,MSpace::kWorld);
		//line.lb = edgeIt.point(1,MSpace::kWorld);

		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = voronoiShatter.orient(v1,v2,v3,line.la);
		orientB =  voronoiShatter.orient(v1,v2,v3,line.lb);

		// both inside or on
		if(orientA *orient>=0 && orientB*orient>=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both outside
		else if(orientA*orient<0 && orientB*orient<0){
			//meshFn.deleteEdge(edgeIt.index());
		}

		// a inside
		else if(orientA*orient>=0){ 
			MPoint intersection = voronoiShatter.getLinePlaneIntersection(line,plane);
			this->displayLocator(intersection);
			intersectionArr.append(intersection);
			//
			int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[5] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = voronoiShatter.getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+ ", factor:" +ft +";\n";
			//
			//edgeIt.setPoint(intersection,1,MSpace::kWorld);
			//edgeItr.setPoint(intersection,1);
		}

		// b inside
		else if(orientB*orient>=0){
			MPoint intersection =  voronoiShatter.getLinePlaneIntersection(line,plane);
			this->displayLocator(intersection);
			intersectionArr.append(intersection);
			//
			int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[5] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = voronoiShatter.getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+ ", factor:" +ft +";\n";
			//
			//edgeIt.setPoint(intersection,0,MSpace::kWorld);
			//edgeItr.setPoint(intersection,0);
		}
		}
		
		meshFn.updateSurface();
		edgeItr.reset();
		vertexItr.reset();
		double e =0.001;
		for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);
		output += MString("NEW") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z+";\n";

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = voronoiShatter.orient(v1,v2,v3,line.la);
		orientB = voronoiShatter.orient(v1,v2,v3,line.lb);

		// delete edge if one vertex outside
		if(abs(orientA)<e)
			orientA =0;
		if(abs(orientB)<e)
			orientB =0;

		// both inside or on
		if(orientA *orient>=0 && orientB*orient>=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both outside
		else if(orientA*orient<0 && orientB*orient<0){
			//meshFn.deleteEdge(edgeIt.index());
		}

		// a inside
		else if(orientA*orient>=0){ 
			//edgeIt.setPoint(intersection,1,MSpace::kWorld);
			edgeItr.setPoint(line.la,1);
		}

		// b inside
		else if(orientB*orient>=0){
			//edgeIt.setPoint(intersection,0,MSpace::kWorld);
			edgeItr.setPoint(line.lb,0);
		}

		edgeItr.updateSurface();

	}
		output += MString("Edge count after split:") + edgeItr.count();
		
	/*		meshFn.updateSurface();
		edgeItr.reset();
		vertexItr.reset();
	
		for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);
		output += MString("NEW") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z+";\n";
		if(abs(line.la.x - line.lb.x)<e
			&&abs(line.la.y - line.lb.y)<e
			&&abs(line.la.z - line.lb.z)<e){
				output += MString("Merage!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z+";\n";
				MIntArray edgeArr;
				edgeArr.append(edgeItr.index());
				meshFn.collapseEdges(edgeArr);
		}
		

	}
	output += MString("Edge count after split:") + edgeItr.count();
*/
	/*meshFn.updateSurface();
	edgeItr.reset();
	vertexItr.reset();
		
	for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);
		output += MString("NEW") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z+";\n";
	}
	output += MString("Edge count after split:") + edgeItr.count();*/
		//meshFn.addPolygon(intersectionArr);
		// clean the vertex which outside the tetra
/*	output += MString("****************************************************************************");
	meshFn.updateSurface();
	edgeItr.reset();
	vertexItr.reset();
	// plane 3
		voronoiShatter.vertexToPlane(tetra.v1,tetra.v2,tetra.v4,plane);
		orient = voronoiShatter.orient(tetra.v1,tetra.v2,tetra.v4,tetra.v3.point);
	for(;!edgeItr.isDone();edgeItr.next()){
		
		edgeItr.updateSurface();

		// set a b
		//line.la = edgeIt.point(0,MSpace::kWorld);
		//line.lb = edgeIt.point(1,MSpace::kWorld);

		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = voronoiShatter.orient(v1,v2,v3,line.la);
		orientB =  voronoiShatter.orient(v1,v2,v3,line.lb);

		// both inside or on
		if(orientA *orient>=0 && orientB*orient>=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both outside
		else if(orientA*orient<0 && orientB*orient<0){
			//meshFn.deleteEdge(edgeIt.index());
		}

		// a inside
		else if(orientA*orient>=0){ 
			MPoint intersection = voronoiShatter.getLinePlaneIntersection(line,plane);
			this->displayLocator(intersection);
			intersectionArr.append(intersection);
			//
			int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[5] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = voronoiShatter.getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+ ", factor:" +ft +";\n";
			//
			//edgeIt.setPoint(intersection,1,MSpace::kWorld);
			//edgeItr.setPoint(intersection,1);
		}

		// b inside
		else if(orientB*orient>=0){
			MPoint intersection =  voronoiShatter.getLinePlaneIntersection(line,plane);
			this->displayLocator(intersection);
			intersectionArr.append(intersection);
			//
			int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[5] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = voronoiShatter.getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+ ", factor:" +ft +";\n";
			//
			//edgeIt.setPoint(intersection,0,MSpace::kWorld);
			//edgeItr.setPoint(intersection,0);
		}
		}
		
		meshFn.updateSurface();
		edgeItr.reset();
		vertexItr.reset();
		 e = 0.000001;
		
		for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);
		output += MString("NEW") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z+";\n";

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = voronoiShatter.orient(v1,v2,v3,line.la);
		orientB = voronoiShatter.orient(v1,v2,v3,line.lb);

		// delete edge if one vertex outside
		if(abs(orientA)<e)
			orientA =0;
		if(abs(orientB)<e)
			orientB =0;

		// both inside or on
		if(orientA *orient>=0 && orientB*orient>=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both outside
		else if(orientA*orient<0 && orientB*orient<0){
			//meshFn.deleteEdge(edgeIt.index());
		}

		// a inside
		else if(orientA*orient>=0){ 
			//edgeIt.setPoint(intersection,1,MSpace::kWorld);
			edgeItr.setPoint(line.la,1);
		}

		// b inside
		else if(orientB*orient>=0){
			//edgeIt.setPoint(intersection,0,MSpace::kWorld);
			edgeItr.setPoint(line.lb,0);
		}

		edgeItr.updateSurface();

	}
		output += MString("Edge count after split:") + edgeItr.count();

		//meshFn.addPolygon(intersectionArr);
		// clean the vertex which outside the tetra
/*		output += MString("****************************************************************************");
	meshFn.updateSurface();
	//MFnMesh meshFn(newDagPath);
	//MItMeshEdge edgeItr(newDagPath);
	edgeItr.reset();
	//MItMeshVertex vertexItr(newDagPath);
	vertexItr.reset();

	for(;!edgeItr.isDone();edgeItr.next()){
		
		edgeItr.updateSurface();

		// set a b
		line.la = edgeItr.point(0,MSpace::kWorld);
		line.lb = edgeItr.point(1,MSpace::kWorld);

		//line.la = edgeItr.point(0);
		//line.lb = edgeItr.point(1);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = voronoiShatter.orient(v1,v2,v3,line.la);
		orientB = voronoiShatter.orient(v1,v2,v3,line.lb);

		// both inside or on
		if(orientA *orient>=0 && orientB*orient>=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both outside
		else if(orientA*orient<0 && orientB*orient<0){
			//meshFn.deleteEdge(edgeItr.index());
		}

		// a inside
		else if(orientA*orient>=0){ 
			MPoint intersection = voronoiShatter.getLinePlaneIntersection(line,plane);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+ ";\n";
			edgeItr.setPoint(intersection,1,MSpace::kWorld);
		}

		// b inside
		else if(orientB*orient>=0){
			MPoint intersection = voronoiShatter.getLinePlaneIntersection(line,plane);
			output += MString("Split!!!") + "A:" + line.la.x + ","+ line.la.y + ","+line.la.z
				+"B:" + line.lb.x + ","+ line.lb.y + ","+line.lb.z 
				+"Insert:" + intersection.x + ","+ intersection.y + ","+intersection.z+";\n";
			edgeItr.setPoint(intersection,0,MSpace::kWorld);
			
		}

	}
	output += MString("Edge count after split:") + edgeItr.count();
	//voronoiShatter.splitMesh(newDagPath,plane,orient);
	*/
//*********************************/
		meshFn.updateSurface();
       // MString cmd = MString("source ktRockShatter;");
		//fDGModifier.commandToExecute(cmd);
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
MObject VoronoiShatterCmd::createTetraMesh(Tetrahedron tetra, MFnMesh &meshFn)
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
void VoronoiShatterCmd::assignShader(MObject &mesh, MFnMesh &meshFn, MDagPath &dagPath)
{
	MObjectArray shaders, comps;
	MFnSet setFn;
	meshFn.setObject(dagPath);
	meshFn.getConnectedSetsAndMembers(dagPath.instanceNumber(),shaders,comps,true);
	setFn.setObject(shaders[0]);
	setFn.addMember(mesh);
}