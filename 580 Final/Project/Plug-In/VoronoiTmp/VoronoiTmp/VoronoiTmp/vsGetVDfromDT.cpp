//======================================================= 
// File: vsGetDTformDT.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"
#include <iostream>
#include <stdio.h>

//	Description:
//
//	Arguments:
//
//	Return Value;

void equation2( double *x, double *y, double a1, double b1, double c1, double a2, double b2, double c2 );
void equation3(double *x, double *y, double *z, double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double a3, double b3, double c3, double d3);

void VoronoiShatter::getVDFormDT(){

	//VD's vertex:  DT's tetra, center of the sphere
	//VD's edge:  2 VD's vertex with the same DT's face
	//VD's face:  tetra which share DT's edge
	//VD's poly:  share DT's vertex
	
	Tetrahedron t;
	VoronoiShatter voronoiShatter;
	TetraMap pool = voronoiShatter.getPool();
	TetraMapItr itr = pool.begin();

	for( ; itr!=pool.end(); itr++){
		// get tetrahedront
		t = itr->second;
	
		// put all verteces to set, not include big tetra's vertecies
		if( t.v1 != bigTetra .v1 && t.v1 != bigTetra .v2 && t.v1 !=bigTetra .v3 && t.v1 != bigTetra .v4)
			vertexSet.insert(t.v1);
		if( t.v2 != bigTetra .v1 && t.v2 != bigTetra .v2 && t.v2 !=bigTetra .v3 && t.v2 != bigTetra .v4)
			vertexSet.insert(t.v2);
		if( t.v3 != bigTetra .v1 && t.v3 != bigTetra .v2 && t.v3 !=bigTetra .v3 && t.v3 != bigTetra .v4)
			vertexSet.insert(t.v3);
		if( t.v4 != bigTetra .v1 && t.v4 != bigTetra .v2 && t.v4 !=bigTetra .v3 && t.v4 != bigTetra .v4)
			vertexSet.insert(t.v4);
	}


	Tetrahedron originalTetra;


	while( !vertexSet.empty() )
	{
		// not sure
		// choose one vertex p
		checkVertex = vertexSet.begin();	

		// if no tetra contains p
		if( getTetra(checkVertex->incidentTetra, originalTetra)  ==false){
			vertexSet.erase(checkVertex);
			continue;
		}
			
		// tetra contains p
		inStackEdge e1;
		inStackEdge e2;
		inStackEdge e3;

		// push the edge to stack
		if(*checkVertex == originalTetra.v1){
			e1.startVertex = originalTetra.v1;
			e1.endVertex = originalTetra.v2;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v1;
			e2.endVertex = originalTetra.v3;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v1;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeStk.push(e1);
			edgeStk.push(e2);
			edgeStk.push(e3);
		}
		else if(*checkVertex == originalTetra.v2){
			e1.startVertex = originalTetra.v2;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v2;
			e2.endVertex = originalTetra.v3;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v2;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeStk.push(e1);
			edgeStk.push(e2);
			edgeStk.push(e3);
		}
		else if(*checkVertex == originalTetra.v3){
			e1.startVertex = originalTetra.v3;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v3;
			e2.endVertex = originalTetra.v2;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v3;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeStk.push(e1);
			edgeStk.push(e2);
			edgeStk.push(e3);
		}
		else if(*checkVertex == originalTetra.v4){
			e1.startVertex = originalTetra.v4;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v4;
			e2.endVertex = originalTetra.v2;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v4;
			e3.endVertex = originalTetra.v3;
			e3.key = originalTetra.key;

			edgeStk.push(e1);
			edgeStk.push(e2);
			edgeStk.push(e3);
		}


		int vdEdgeTag = 0;
		int vdFaceTag = 0;
		VDfaceIndex.push_back(0);
		VDpolyIndex.push_back(0);

		while( !edgeStk.empty() )
		{

			int previousTetraKey = 0;
			int nextTetraKey = 0;
			int firstTetraKey = 0;
			int currentKey = 0;
			bool hasNeighbor = true;
			Tetrahedron currentTetra;

			// choose one edge E
			inStackEdge currentEdge;
			currentEdge = edgeStk.top();

			// find the edge from which tetra
			currentKey = currentEdge.key;
			firstTetraKey = currentEdge.key;
			edgeStk.pop();

			while( hasNeighbor )
			{
		
				// get current tetra
				if( getTetra( currentKey, currentTetra)  ==false){
					continue;
				}


				// find VD's vertex vp of currentTetra
				Vertex vp;
				vp = findSphereCenter( currentTetra );
				// end of finding VD's vertex vp of currentTetra


				// put vp into VDvertex
				VDvertex.push_back(vp);
				// end of putting vp into VDvertex

				// recode the VDedge
				if(VDvertex.size()!=1)
				{
					Edge vdEdge;
					vdEdge.startVertexId = VDvertex.size() - 2;
					vdEdge.endVertexId = VDvertex.size() - 1;
					VDedge.push_back( vdEdge );
				}
				// end record the VDedge


				// save now's tetra for preious
				previousTetraKey = currentTetra.key;

				// choose neighbor tetra which contain edge e
				Tetrahedron t1, t2, t3, t4;
				if( getTetra(currentTetra.neighbour1, t1) == false )
					std::cerr<<"no neighbor1"<<std::endl;
				if( getTetra(currentTetra.neighbour2, t2) == false )
					std::cerr<<"no neighbor2"<<std::endl;
				if( getTetra(currentTetra.neighbour3, t3) == false )
					std::cerr<<"no neighbor3"<<std::endl;
				if( getTetra(currentTetra.neighbour4, t4) == false )
					std::cerr<<"no neighbor4"<<std::endl;

				// if tetra contains edge, and it is not the first tetra
				if ( checkEdge(t1, currentEdge) == true && t1.key != previousTetraKey ){
					currentKey = t1.key;
				}
				else if ( checkEdge(t2, currentEdge) == true && t2.key != previousTetraKey ){
					currentKey = t2.key;
				}
				else if ( checkEdge(t3, currentEdge) == true && t3.key != previousTetraKey ){
					currentKey = t3.key;
				}
				else if ( checkEdge(t4, currentEdge) == true && t4.key != previousTetraKey ){
					currentKey = t4.key;
				}
				else{
					hasNeighbor = false;
				}

			}// end while of find neighbor

			// this is a VDface
			for(int i=vdEdgeTag; i<VDedge.size(); i++)
			{
				VDface.push_back( i );
			}

			vdEdgeTag = VDedge.size();
			VDfaceIndex.push_back( vdEdgeTag );
			// end of VDface

		}//end while edge stk

		//this is a VDpoly
		for(int i=vdFaceTag; i<VDface.size(); i++)
		{
			VDpoly.push_back( i );
		}

		vdFaceTag = VDface.size();
		VDpolyIndex.push_back( vdFaceTag );
		//end if VDpoly

		// erase the vertex
		vertexSet.erase( checkVertex );

	}//end while of vertex stk
	return;
}



bool VoronoiShatter::checkEdge(Tetrahedron t, inStackEdge e)
{
	Vertex p1, p2, p3, p4;
	p1 = t.v1;
	p2 = t.v2;
	p3 = t.v3;
	p4 = t.v4;

	if( e.startVertex == p1 )
	{
		if( e.endVertex == p2 || e.endVertex ==p3 || e.endVertex ==p4 )
			return true;
	}
	else if( e.startVertex == p2 )
	{
		if( e.endVertex == p1 || e.endVertex ==p3 || e.endVertex ==p4 )
			return true;
	}
	else if( e.startVertex == p3 )
	{
		if( e.endVertex == p1 || e.endVertex ==p2 || e.endVertex ==p4 )
			return true;
	}
	else if(e.startVertex == p4)
	{
		if( e.endVertex == p1 || e.endVertex ==p2 || e.endVertex ==p3 )
			return true;
	}
	else
		return false;

}


void equation2(double *x, double *y, double a1, double b1, double c1, double a2, double b2, double c2)
{
*x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
*y = (c1 - a1 * *x) / b1;

}

void equation3(double *x, double *y, double *z, double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double a3, double b3, double c3, double d3)
{
equation2(x, y, a1 * c2 - a2 * c1, b1 * c2 - b2 * c1, d1 * c2 - d2 * c1, a1 * c3 - a3 * c1, b1 * c3 - b3 * c1, d1 * c3 - d3 * c1);
*z = (d1 - a1 * *x - b1 * *y) / c1;

}



Vertex VoronoiShatter::findSphereCenter( Tetrahedron t )
{
/*
//		a1*X + b1*Y + c1*Z = d1
//		a2*X + b2*Y + c2*Z = d2
//		a3*X + b3*Y + c3*Z = d3
//
//		now: center (a, b, c),  4 vertex (x1, y1, z1)    (x2, y2, z2)    (x3, y3, z3)    (x4, y4, z4)
//
//		(-2*x1 + 2*x2) * a  +  (-2*y1 + 2*y2) * b  +  (-2*z1 + 2*z2) * c = (x2)^2 - (x1)^2 + (y2)^2 - (y1)^2 + (z2)^2 - (z1)^2             use 1 = 2
//		(-2*x2 + 2*x3) * a  +  (-2*y2 + 2*y3) * b  +  (-2*z2 + 2*z3) * c = (x3)^2 - (x2)^2 + (y3)^2 - (y2)^2 + (z3)^2 - (z2)^2             use 2 = 3
//		(-2*x3 + 2*x4) * a  +  (-2*y3 + 2*y4) * b  +  (-2*z3 + 2*z4) * c = (x4)^2 - (x3)^2 + (y4)^2 - (y3)^2 + (z4)^2 - (z3)^2             use 3 = 4
//
//		X^2 - Y^2 = (X+Y) * (X-Y)
*/

	Vertex center;
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double x4, y4, z4;

	x1 = t.v1.point.x;
	y1 = t.v1.point.y;
	z1 = t.v1.point.z;

	x2 = t.v2.point.x;
	y2 = t.v2.point.y;
	z2 = t.v2.point.z;

	x3 = t.v3.point.x;
	y3 = t.v3.point.y;
	z3 = t.v3.point.z;

	x4 = t.v4.point.x;
	y4 = t.v4.point.y;
	z4 = t.v4.point.z;



	double x, y, z;
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;

	a1 = ( -2*x1 + 2*x2 );
	b1 = ( -2*y1 + 2*y2 );
	c1 = ( -2*z1 + 2*z2 );
	d1 = ( x2 + x1 )*( x2 - x1 ) + ( y2 + y1 )*( y2 - y1 ) + ( z2 + z1 )*( z2 - z1 );

	a2 = ( -2*x2 + 2*x3 );
	b2 = ( -2*y2 + 2*y3 );
	c2 = ( -2*z2 + 2*z3 );
	d2 = ( x3 + x2 )*( x3 - x2 ) + ( y3 + y2 )*( y3 - y2 ) + ( z3 + z2 )*( z3 - z2 );

	a3 = ( -2*x3 + 2*x4 );
	b3 = ( -2*y3 + 2*y4 );
	c3 = ( -2*z3 + 2*z4 );
	d3 = ( x4 + x3 )*( x4 - x3 ) + ( y4 + y3 )*( y4 - y3 ) + ( z4 + z3 )*( z4 - z3 );

	equation3(&x, &y, &z, a1, b1, c1, d1, a2, b2, c2, d2, a3, b3, c3, d3);

	center.point.x = x;
	center.point.y = y;
	center.point.z = z;

	return center;
}