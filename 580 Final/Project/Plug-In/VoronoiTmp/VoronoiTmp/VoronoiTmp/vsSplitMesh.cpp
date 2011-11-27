//======================================================= 
// File: vsSplitMesh.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//		split mesh using a plane
//	Arguments:
//		plane -- plane used to split mesh
//	Return Value;
void VoronoiShatter::splitMesh(MDagPath &dagPath, Plane plane)
{
	//MDagPath mesh = vMesh;
	MFnMesh meshFn(dagPath);
	MItMeshEdge edgeIt(dagPath);

	Line line;
	for(;!edgeIt.isDone();edgeIt.next()){
		
		// set a b
		line.la = edgeIt.point(0,MSpace::kWorld);
		line.lb = edgeIt.point(1,MSpace::kWorld);

		int orientA, orientB;
		Vertex v1,v2,v3;
		
		v1.point = plane.pa;
		v2.point = plane.pb;
		v3.point = plane.pc;

		orientA = orient(v1,v2,v3,line.la);
		orientB = orient(v1,v2,v3,line.lb);

		// both under or on
		if(orientA<=0 && orientB<=0){
			// a on
			if(orientA==0){
			}
			// b on
			if(orientB==0){
			}
		}

		// both above
		else if(orientA>0 && orientB>0){
			meshFn.deleteEdge(edgeIt.index());
		}

		// a under
		else if(orientA<=0){
			if(orientA == 0){
			}
			else{ 
				MPoint intersection = getLinePlaneIntersection(line,plane);
				edgeIt.setPoint(intersection,1,MSpace::kWorld);
			}
		}

		// b under
		else if(orientB<=0){
			if(orientB==0){
			}
			else{
				MPoint intersection = getLinePlaneIntersection(line,plane);
				edgeIt.setPoint(intersection,0,MSpace::kWorld);
			}
		}

	}
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

	rst.x = (B*(dy/dx)*line.la.x + C*(dz/dx)*line.la.x - B*line.la.y - C*line.la.z -D)/(A + B*(dy/dx) + C*(dz/dx)); 
	rst.y = (dy/dx)*(rst.x - line.la.x) + line.la.y;
	rst.z = (dz/dx)*(rst.x - line.la.x) + line.la.z;

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