//======================================================= 
// File: vsInsertOnePoint.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//
//	Arguments:
//
//	Return Value;
void VoronoiShatter::insertOnePoint(MPoint p){

	//get a tetrahedron t
	Tetrahedron t; 

	if( findPoint(p, t) == false ) return;
	cout<<"p: "<<p.x<<","<<p.y<<","<<p.z<<endl;
	
	//cancel insert if p is directly on a face
	if( orient( t.v1, t.v2, t.v3, p ) == 0 ) return;
	if( orient( t.v1, t.v2, t.v4, p ) == 0 ) return;
	if( orient( t.v1, t.v3, t.v4, p ) == 0 ) return;
	if( orient( t.v2, t.v3, t.v4, p ) == 0 ) return;

	//t -> do flip14
	//flip14 only happens here
	Tetrahedron t1, t2, t3, t4;

	t1.v1 = t.v1;
	t1.v2 = t.v2;
	t1.v3 = t.v3;
	t1.v4.point = p;
	addTetra(t1);

	t2.v1 = t.v2;
	t2.v2 = t.v3;
	t2.v3 = t.v4;
	t2.v4.point = p;
	addTetra(t2);

	t3.v1 = t.v1;
	t3.v2 = t.v3;
	t3.v3 = t.v4;
	t3.v4.point = p;
	addTetra(t3);

	t4.v1 = t.v1;
	t4.v2 = t.v2;
	t4.v3 = t.v4;
	t4.v4.point = p;
	addTetra(t4);

	// set neighbors of t1, t2, t3, t4 	
	t1.neighbour1 = getNeighborByVertices( t, t1.v1.point, t1.v2.point, t1.v3.point );
	t1.neighbour2 = t2.key;
	t1.neighbour3 = t3.key;
	t1.neighbour4 = t4.key;

	t2.neighbour1 = getNeighborByVertices( t, t2.v1.point, t2.v2.point, t2.v3.point );
	t2.neighbour2 = t1.key;
	t2.neighbour3 = t3.key;
	t2.neighbour4 = t4.key;

	t3.neighbour1 = getNeighborByVertices( t, t3.v1.point, t3.v2.point, t3.v3.point );
	t3.neighbour2 = t1.key;
	t3.neighbour3 = t2.key;
	t3.neighbour4 = t4.key;

	t4.neighbour1 = getNeighborByVertices( t, t4.v1.point, t4.v2.point, t4.v3.point );
	t4.neighbour2 = t1.key;
	t4.neighbour3 = t2.key;
	t4.neighbour4 = t3.key;

	// update t1, t2, t3, t4 in map
	updateTetra(t1);
	updateTetra(t2);
	updateTetra(t3);
	updateTetra(t4);

	// update the neighbours of t
	Tetrahedron n;

	if( getTetra( t1.neighbour1, n ) ) {
		replaceNeighbour( n, t.key, t1.key );
		updateTetra(n);
	}

	if( getTetra( t2.neighbour1, n ) ) {
		replaceNeighbour( n, t.key, t2.key );
		updateTetra(n);
	}

	if( getTetra( t3.neighbour1, n ) ) {
		replaceNeighbour( n, t.key, t3.key );
		updateTetra(n);
	}

	if( getTetra( t4.neighbour1, n ) ) {
		replaceNeighbour( n, t.key, t4.key );
		updateTetra(n);
	}

	// finally delete t
	deleteTetra(t.key);

	// push new tetras on stack
	flipStack.push( t1.key );
	flipStack.push( t2.key );
	flipStack.push( t3.key );
	flipStack.push( t4.key );

	while( !flipStack.empty() )
	{
		//pop one tetrahedron from stack
		int flipkey = flipStack.top();
		flipStack.pop();

		//flip
		flip( flipkey, p );
	}

	return;
}

int VoronoiShatter::getNeighborByVertices( Tetrahedron &t, MPoint a, MPoint b, MPoint c )
{
	Tetrahedron tmp;

	if( getTetra(t.neighbour1, tmp) ) {
		if( isInTetrahedron(tmp, a) && isInTetrahedron(tmp, b) && isInTetrahedron(tmp, c) )
			return tmp.key;
	}

	if( getTetra(t.neighbour2, tmp) ) {
		if( isInTetrahedron(tmp, a) && isInTetrahedron(tmp, b) && isInTetrahedron(tmp, c) )
			return tmp.key;
	}

	if( getTetra(t.neighbour3, tmp) ) {
		if( isInTetrahedron(tmp, a) && isInTetrahedron(tmp, b) && isInTetrahedron(tmp, c) )
			return tmp.key;
	}
	
	if( getTetra(t.neighbour4, tmp) ) {
		if( isInTetrahedron(tmp, a) && isInTetrahedron(tmp, b) && isInTetrahedron(tmp, c) )
			return tmp.key;
	}

	return -1;
}

bool VoronoiShatter::isInTetrahedron( Tetrahedron &tetra, MPoint v )
{
	return (v == tetra.v1.point) ||
		(v == tetra.v2.point) ||
		(v == tetra.v3.point) ||
		(v == tetra.v4.point);
}