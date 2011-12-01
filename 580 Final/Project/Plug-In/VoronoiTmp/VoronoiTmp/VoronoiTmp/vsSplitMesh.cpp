//======================================================= 
// File: vsSplitMesh.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"
#include <maya/MFloatPointArray.h>
#include <maya/MPointArray.h>
#include <maya/MIntArray.h>
#include <math.h>

float VoronoiShatter::getFactor(MPoint a, MPoint b, MPoint insert){
	float factor;
	factor = sqrt(pow((insert.x - a.x),2) + pow((insert.y - a.y),2)+pow((insert.z - a.z),2))/
		sqrt(pow((b.x - a.x),2) + pow((b.y - a.y),2)+pow((b.z - a.z),2));
	
	return factor;
}
//	Description:
//		split mesh using a plane
//	Arguments:
//		plane -- plane used to split mesh
//	Return Value;
void VoronoiShatter::vertexToPlane(Vertex v1, Vertex v2, Vertex v3, Plane &plane)
{
	plane.pa = v1.point;
	plane.pb = v2.point;
	plane.pc = v3.point;
}

//	Description:
//		split mesh using a plane
//	Arguments:
//		plane -- plane used to split mesh
//	Return Value;
void VoronoiShatter::split(MDagPath &dagPath, Tetrahedron tetra)
{
	MFnDagNode nodeFn(dagPath);
	int orient;
	Plane plane;

	// plane 1 
	vertexToPlane(tetra.v1,tetra.v2,tetra.v3,plane);
	orient = this->orient(tetra.v1,tetra.v2,tetra.v3,tetra.v4.point);
	this->splitMesh(dagPath,plane,orient);
	
	// plane 2
	vertexToPlane(tetra.v1,tetra.v2,tetra.v4,plane);
	orient = this->orient(tetra.v1,tetra.v2,tetra.v4,tetra.v3.point);
	this->splitMesh(dagPath,plane,orient);
	
	// plane 3
	vertexToPlane(tetra.v2,tetra.v3,tetra.v4,plane);
	orient = this->orient(tetra.v2,tetra.v3,tetra.v4,tetra.v1.point);
	this->splitMesh(dagPath,plane,orient);
	
	// plane 4
	vertexToPlane(tetra.v1,tetra.v3,tetra.v4,plane);
	orient = this->orient(tetra.v1,tetra.v3,tetra.v4,tetra.v2.point);
	this->splitMesh(dagPath,plane,orient);
}


//	Description:
//		split mesh using a plane
//	Arguments:
//		plane  -- plane used to split mesh
//		orient -- which orient will be considered as inside
//	Return Value;
void VoronoiShatter::splitMesh(MDagPath &mesh, Plane plane, int orient)
{
	//nodeFn.duplicate();
	MFnMesh meshFn(mesh);
	MItMeshEdge edgeItr(mesh);
	MItMeshVertex vertexItr(mesh);
	//MItMeshEdge edgeItr4Del(newDagPath);
	//MItMeshVertex vertexItr4Del(newDagPath);

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

		orientA = this->orient(v1,v2,v3,line.la);
		orientB =  this->orient(v1,v2,v3,line.lb);

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
			MPoint intersection = getLinePlaneIntersection(line,plane);
			//this->displayLocator(intersection);
			//intersectionArr.append(intersection);
			//
		/*	int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[1] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);*/
			edgeItr.setPoint(intersection,1);
		}

		// b inside
		else if(orientB*orient>=0){
			MPoint intersection = getLinePlaneIntersection(line,plane);
			//
		/*	int place[1] = {MFnMesh::kOnEdge};
			MIntArray placements(place,1);

			int list[1] = {edgeItr.index()};
			MIntArray edgeList(list,1);
		
			float ft = getFactor(line.la,line.lb,intersection);
			float factor[1] = {ft};
			MFloatArray edgeFactors(factor,1);

			MFloatPointArray internalpoints;

			meshFn.split(placements,edgeList,edgeFactors,internalpoints);*/
			edgeItr.setPoint(intersection,0);
		}
	}
/*	meshFn.updateSurface();
	edgeItr.reset();
	vertexItr.reset();
	double e = 0.001;
		
	for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = this->orient(v1,v2,v3,line.la);
		orientB = this->orient(v1,v2,v3,line.lb);

		// delete edge if one vertex outside
		if(abs(orientA)<e)
			orientA =0;
		if(abs(orientB)<e)
			orientB =0;

		if(orientA *orient==0 && orientB*orient<0){
			edgeItr.setPoint(line.la,1,MSpace::kWorld);
			MIntArray mEdge;
			mEdge.append(edgeItr.index());
			//meshFn.collapseEdges(mEdge);
		}
		if(orientA *orient<0 && orientB*orient==0){
			edgeItr.setPoint(line.lb,0,MSpace::kWorld);
			MIntArray mEdge;
			mEdge.append(edgeItr.index());
			//meshFn.collapseEdges(mEdge);
		}

		edgeItr.updateSurface();

	}

		meshFn.updateSurface();
	/*	edgeItr.reset();
		vertexItr.reset();
		
		for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);
		if(abs(line.la.x - line.lb.x)<e
			&&abs(line.la.y - line.lb.y)<e
			&&abs(line.la.z - line.lb.z)<e){
				MIntArray edgeArr;
				edgeArr.append(edgeItr.index());
				meshFn.collapseEdges(edgeArr);
		}
		

	}

	for(;!edgeItr.isDone();edgeItr.next()){
		line.la = edgeItr.point(0);
		line.lb = edgeItr.point(1);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = this->orient(v1,v2,v3,line.la);
		orientB = this->orient(v1,v2,v3,line.lb);

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
			MPoint intersection = getLinePlaneIntersection(line,plane);
			edgeItr.setPoint(intersection,1);
		}

		// b inside
		else if(orientB*orient>=0){
			MPoint intersection = getLinePlaneIntersection(line,plane);
			edgeItr.setPoint(intersection,0);
		}

		edgeItr.updateSurface();

	}*/
/*	
	meshFn.updateSurface();
	edgeItr.reset();
	vertexItr.reset();
	for(;!edgeItr.isDone();edgeItr.next()){
		edgeItr.updateSurface();
		if(edgeItr.point(0).x == edgeItr.point(1).x
				&&edgeItr.point(0).y == edgeItr.point(1).y
				&&edgeItr.point(0).z == edgeItr.point(1).z)
				meshFn.deleteEdge(edgeItr.index());
	}*/
}

//	Description:
//		get point of intersection of a line and a plane
//	Arguments:
//		line  -- line
//		plane -- plane
//	Return Value;
//		point of intersection
MPoint VoronoiShatter::getLinePlaneIntersection(Line line, Plane plane)
{
	MPoint rst;
	double dx, dy, dz;
	double A,B,C,D;
	Vector AB,AC, N;

	AB.x = plane.pb.x - plane.pa.x;
	AB.y = plane.pb.y - plane.pa.y;
	AB.z = plane.pb.z - plane.pa.z;

	AC.x = plane.pc.x - plane.pa.x;
	AC.y = plane.pc.y - plane.pa.y;
	AC.z = plane.pc.z - plane.pa.z;

	N = crossProduct(AB, AC);

	A = N.x;
	B = N.y;
	C = N.z;
	D = -1*(A*plane.pa.x + B*plane.pa.y + C*plane.pa.z);

	dx = line.lb.x - line.la.x;
	dy = line.lb.y - line.la.y;
	dz = line.lb.z - line.la.z;

	if(dx == 0)
		rst.x = line.la.x;
	else{
		rst.x = (B*(dy/dx)*line.la.x + C*(dz/dx)*line.la.x - B*line.la.y - C*line.la.z -D)/(A + B*(dy/dx) + C*(dz/dx)); 
	    rst.y = (dy/dx)*(rst.x - line.la.x) + line.la.y;
	    rst.z = (dz/dx)*(rst.x - line.la.x) + line.la.z;
		return rst;
	}
	if(dy == 0)
		rst.y = line.la.y;
	else
	{
		rst.y = (C*(dz/dy)*line.la.y- C*line.la.z - A*line.la.x - D)/(B + C*(dz/dy));
		rst.z = (dz/dy)*(rst.y-line.la.y) + line.la.z;
		return rst;
	}
	if(dz == 0)
		rst.z = line.la.z;
	else
	{
		rst.z = (-A*line.la.x - B*line.la.y - D)/C;
		return rst;
	}

	return rst;
}

//	Description:
//		cross product of two vectors
//	Arguments:
//		v1  -- v1
//		v2  -- v2
//	Return Value;
//		result vector
Vector VoronoiShatter::crossProduct(Vector v1, Vector v2)
{
	Vector rst;

	rst.x = v1.y*v2.z - v1.z*v2.y;
	rst.y = v1.z*v2.x - v1.x*v2.z;
	rst.z = v1.x*v2.y - v1.y*v2.x;
	
	return rst;
}