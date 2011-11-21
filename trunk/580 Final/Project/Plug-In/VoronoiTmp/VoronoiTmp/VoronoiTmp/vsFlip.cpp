//======================================================= 
// File: vsFlip.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//
//	Arguments:
//
//	Return Value;
bool VoronoiShatter::flip( int key, MPoint p ){

	Tetrahedron t, ta;
	Vertex a,b,c,d;

	if( getTetra( key, t ) );
	else return false;
	
	// find abc
	if( t.v1.point == p )
	{
		a = t.v2;
		b = t.v3;
		c = t.v4;
	}
	else if( t.v2.point == p )
	{
		a = t.v1;
		b = t.v3;
		c = t.v4;
	}
	else if( t.v3.point == p )
	{
		a = t.v1;
		b = t.v2;
		c = t.v4;
	}
	else if( t.v4.point == p )
	{
		a = t.v1;
		b = t.v2;
		c = t.v3;
	}
	else return false;

	//Tetrahedron ta;

	// get neighbour that share abc
	switch( getNeighborByVertices( t, a.point, b.point, c.point ) )
	{
	case 1:
		if( getTetra( t.neighbour1, ta ) );
		else return false;
		break;
	case 2:
		if( getTetra( t.neighbour2, ta ) );
		else return false;
		break;
	case 3:
		if( getTetra( t.neighbour3, ta ) );
		else return false;
		break;
	case 4:
		if( getTetra( t.neighbour4, ta ) );
		else return false;
		break;
	case -1:
		//no such neightbour, return
		return false;
	}

	// determine d
	switch( getd( ta, a.point, b.point, c.point ) )
	{
	case 1:
		d = ta.v1;
		break;
	case 2:
		d = ta.v2;
		break;
	case 3:
		d = ta.v3;
		break;
	case 4:
		d = ta.v4;
		break;
	case -1:
		//can't find d.
		return false;
	}

	// if d is outside the circumsphere, don't flip
	if( inSphere( t.v1, t.v2, t.v3, t.v4, d.point ) <= 0 )
		return true;

	// determine cases
	int flipCase = 0;

	/*** my notes ***/
	//case 1:
	// point is outside polyhedron and not case 3 or 4
	//case 2:
	// one point is inside the polyhedron formed by other 3
	//case 3:
	// one set of 4 points coplanar (pdab, pdac, pdbc)
	// = pd intersects one edge of abc
	// *test coplanar, if coplanar set = 1 -> case3, 2->case4
	//case 4:
	// two set of 4 points coplanar (pdab, pdac, pdbc)
	// = p is on one of the edges of abc

	// Test case #3 and #4 first.
	// check if these sets are coplanar
	bool pdab, pdac, pdbc;
	Vertex tmp;

	pdab = ( orient( a, b, d, p ) == 0 );
	pdac = ( orient( a, c, d, p ) == 0 );
	pdbc = ( orient( b, c, d, p ) == 0 );

	switch( pdab + pdac + pdbc )
	{
	case 0:
		//not case #3 or #4
		break;
	case 1:
		//case#3
		//swap abc so pdab is coplanar
		if( pdab );
		else if( pdac ) {
			tmp = c;
			c = b;
			b = tmp;
		}
		else if( pdbc ) {
			tmp = c;
			c = a;
			a = tmp;
		}
		else; //assert

		flipCase = 3;
		break;
	case 2:
		//case#4
		flipCase = 4;
		break;
	}

	// if not case#3 or #4
	// test whether #1 or #2
	if( flipCase == 0 )
	{
		if( ( orient( a, b, d, p ) * orient( a, b, d, c.point ) > 0 ) &&
			( orient( a, c, d, p ) * orient( a, c, d, b.point ) > 0 ) &&
			( orient( b, c, d, p ) * orient( b, c, d, a.point ) > 0 ) )	{
			flipCase = 1;
		}
		else flipCase = 2;
	}

	// now the cases are decided.
	// happy flipping time
	bool result;

	switch( flipCase )
	{
	case 1:
	case 4:
		result = flip23( t, ta, a.point, b.point, c.point, d.point, p );
		break;
	case 2:
		result = flip32( t, ta, a.point, b.point, c.point, d.point, p );
		break;
	case 3:
		result = flip44( t, ta, a.point, b.point, c.point, d.point, p );
		break;	
	}

	return result;
}

bool VoronoiShatter::flip44( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p )
{
	// abc is already swapped so that pdab is coplanar
	// test config 44 =  find tb, tc
	Tetrahedron tb, tc;

	//abpk
	if( getTetra( getNeighborByVertices( t, a, b, p ), tb ) );
	else return false;
	//abdk
	if( getTetra( getNeighborByVertices( ta, a, b, d ) , tc ));
	else return false;

	MPoint kb, kc;
	switch( getd( tb, a, b, p ) )
	{
	case 1:
		kb = tb.v1.point;
		break;
	case 2:
		kb = tb.v2.point;
		break;
	case 3:
		kb = tb.v3.point;
		break;
	case 4:
		kb = tb.v4.point;
		break;
	case -1:
		return false;
	}

	switch( getd( tc, a, b, d ) )
	{
	case 1:
		kc = tc.v1.point;
		break;
	case 2:
		kc = tc.v2.point;
		break;
	case 3:
		kc = tc.v3.point;
		break;
	case 4:
		kc = tc.v4.point;
		break;
	case -1:
		return false;
	}

	// t, ta, tb, tc are NOT in config 44.
	if( kb != kc ) return true;
	// else they are.

	//create 4 new tetrahedron
	Tetrahedron ckad, ckap, ckbd, ckbp;

	ckad.v1.point = c;
	ckad.v2.point = kc;
	ckad.v3.point = a;
	ckad.v4.point = d;
	addTetra( ckad );

	ckap.v1.point = c;
	ckap.v2.point = kc;
	ckap.v3.point = a;
	ckap.v4.point = p;
	addTetra( ckap );

	ckbd.v1.point = c;
	ckbd.v2.point = kc;
	ckbd.v3.point = b;
	ckbd.v4.point = d;
	addTetra( ckbd );

	ckbp.v1.point = c;
	ckbp.v2.point = kc;
	ckbp.v3.point = b;
	ckbp.v4.point = p;
	addTetra( ckbp );

	//set new tetra's neighbour 
	//note: tc = abdk, tb= abpk

	//ckad
	ckad.neighbour1 = ckap.key; //cka
	ckad.neighbour2 = ckbd.key; //ckd
	ckad.neighbour3 = getNeighborByVertices( ta, a, c, d ); //acd
	ckad.neighbour4 = getNeighborByVertices( tc, a, d, kc ); //adk
	updateTetra(ckad);

	//ckap
	ckap.neighbour1 = ckad.key; //cka
	ckap.neighbour2 = ckbp.key; //ckp
	ckap.neighbour3 = getNeighborByVertices( t, a, c, p ); //acp
	ckap.neighbour4 = getNeighborByVertices( tb, a, p, kc ); //apk
	updateTetra(ckap);

	//ckbd
	ckbd.neighbour1 = ckad.key; //ckd
	ckbd.neighbour2 = ckbp.key; //ckb
	ckbd.neighbour3 = getNeighborByVertices( ta, b, c, d ); //bcd
	ckbd.neighbour4 = getNeighborByVertices( tc, b, d, kc ); //bdk
	updateTetra(ckbd);

	//ckbp
	ckbp.neighbour1 = ckbd.key; //ckb
	ckbp.neighbour2 = ckap.key; //ckp
	ckbp.neighbour3 = getNeighborByVertices( t, b, c, p ); //bcp
	ckbp.neighbour4 = getNeighborByVertices( tb, b, p, kc ); //bpk
	updateTetra(ckbp);

	// done with new tetras

	// update old tetras //note: tc = abdk, tb= abpk
	Tetrahedron n;

	// start with neighbours of t
	if( getTetra( getNeighborByVertices( t, p, a, c ), n ) ) {
		replaceNeighbour( n, t.key, ckap.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( t, p, b, c ), n ) ) {
		replaceNeighbour( n, t.key, ckbp.key );
		updateTetra(n);
	}
	// then ta
	if( getTetra( getNeighborByVertices( ta, b, c, d ), n ) ) {
		replaceNeighbour( n, ta.key, ckbd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( ta, a, c, d ), n ) ) {
		replaceNeighbour( n, ta.key, ckad.key );
		updateTetra(n);
	}
	// then tb
	if( getTetra( getNeighborByVertices( tb, a, p, kc ), n ) ) {
		replaceNeighbour( n, tb.key, ckap.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( tb, b, p, kc ), n ) ) {
		replaceNeighbour( n, tb.key, ckbp.key );
		updateTetra(n);
	}
	// finally tc
	if( getTetra( getNeighborByVertices( tc, a, d, kc ), n ) ) {
		replaceNeighbour( n, tc.key, ckad.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( tc, b, d, kc ), n ) ) {
		replaceNeighbour( n, tc.key, ckbd.key );
		updateTetra(n);
	}

	// done with old tetras

	// delete flipped tetras
	deleteTetra( t.key );
	deleteTetra( ta.key );
	deleteTetra( tb.key );
	deleteTetra( tc.key );

	//push new tetras to stack
	flipStack.push( ckad.key );
	flipStack.push( ckap.key );
	flipStack.push( ckbd.key );
	flipStack.push( ckbp.key );

	return true;
	
}
bool VoronoiShatter::flip32( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p )
{

	// find pdab (or maybe pdac, pdbc)
	Tetrahedron third;
	int found = 0;

	// test pdab first
	if( getTetra( getNeighborByVertices( t, p, a, b ), third ) );
	return false;

	if( isInTetrahedron( third, d ) ) found = 1;

	// if not found, pdac
	if( found == 0 ) {
		if( getTetra( getNeighborByVertices( t, p, a, c ), third ) );
		else return false;

		if( isInTetrahedron( third, d ) ) found = 2;
	}
	// then pdbc
	if( found == 0 ) {
		if( getTetra( getNeighborByVertices( t, p, b, c ), third ) );
		else return false;

		if( isInTetrahedron( third, d ) ) found = 3;
	}
	
	// no flip if there is no such tetrahedron
	if( found == 0 ) return true;

	// found = 1 : pacd & pbcd
	// found = 2 : pabd & pbcd
	// found = 3 : pabd & pacd
	MPoint tmp;

	// swap abc to match case 1
	switch( found )
	{
	case 1:
		// pdab
		break;
	case 2:
		// pdac
		// swap b and c
		tmp = c;
		c = b;
		b = tmp;
		break;
	case 3:
		// pdbc
		// swap a and c
		tmp = c;
		c = a;
		a = tmp;
		break;
	}

	// create 2 new tetrahedron
	Tetrahedron pacd, pbcd; //pabd;

	//pbcd
	pbcd.v1.point = p;
	pbcd.v2.point = b;
	pbcd.v3.point = c;
	pbcd.v4.point = d;
	addTetra( pbcd );
	//pacd
	pacd.v1.point = p;
	pacd.v2.point = a;
	pacd.v3.point = c;
	pacd.v4.point = d;
	addTetra( pacd );

	// set new tetra's neighbour

	//update pacd's neighbours
	pacd.neighbour1 = pbcd.key; //pcd
	pacd.neighbour2 = getNeighborByVertices( t, p, a, c ); //pac
	pacd.neighbour3 = getNeighborByVertices( ta, a, c, d ); //acd
	pacd.neighbour4 = getNeighborByVertices( third, p, a, d ); //pad
	updateTetra(pacd);

	//update pbcd's neighbours
	pbcd.neighbour1 = pacd.key; //pcd
	pbcd.neighbour2 = getNeighborByVertices( t, p, b, c ); //pbc
	pbcd.neighbour3 = getNeighborByVertices( ta, b, c, d ); //bcd
	pbcd.neighbour4 = getNeighborByVertices( third, p, b, d ); //pbd
	updateTetra(pbcd);

	// done with new tetras

	// update neighbours of old tetras
	Tetrahedron n;

	// start with neighbours of t
	if( getTetra( getNeighborByVertices( t, p, a, c ), n ) ) {
		replaceNeighbour( n, t.key, pacd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( t, p, b, c ), n ) ) {
		replaceNeighbour( n, t.key, pbcd.key );
		updateTetra(n);
	}

	// then ta
	if( getTetra( getNeighborByVertices( ta, d, a, c ), n ) ) {
		replaceNeighbour( n, ta.key, pacd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( ta, d, b, c ), n ) ) {
		replaceNeighbour( n, ta.key, pbcd.key );
		updateTetra(n);
	}

	// then third
	if( getTetra( getNeighborByVertices( third, p, a, d ), n ) ) {
		replaceNeighbour( n, third.key, pacd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( third, p, b, d ), n ) {
		replaceNeighbour( n, third.key, pbcd.key );
		updateTetra(n);
	}

	// delete t,ta,third
	deleteTetra( t.key );
	deleteTetra( ta.key );
	deleteTetra( third.key );

	// push new tetras
	flipStack.push( pacd.key );
	flipStack.push( pbcd.key );

	return true;
	
}

bool VoronoiShatter::flip23( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p )
{

	// create 3 new tetrahedra
	// and set the neightbours of new tetrahedra WTF
	Tetrahedron pabd, pbcd, pacd;

	pabd.v1.point = p;
	pabd.v2.point = a;
	pabd.v3.point = b;
	pabd.v4.point = d;

	pabd.neighbour1 = getNeighborByVertices(t, a, b, p);
	pabd.neighbour2 = getNeighborByVertices(ta, a, b, d);
	//neighbour 3 and 4 are pbcd and pacd
	addTetra( pabd );

	pbcd.v1.point = p;
	pbcd.v2.point = b;
	pbcd.v3.point = c;
	pbcd.v4.point = d;

	pbcd.neighbour1 = getNeighborByVertices(t, c, b, p);
	pbcd.neighbour2 = getNeighborByVertices(ta, c, b, d);
	pbcd.neighbour3 = pabd.key;
	//neighbour 4 is pacd
	addTetra( pbcd );

	pacd.v1.point = p;
	pacd.v2.point = a;
	pacd.v3.point = c;
	pacd.v4.point = d;

	pacd.neighbour1 = getNeighborByVertices(t, a, c, p);
	pacd.neighbour2 = getNeighborByVertices(ta, a, c, d);
	pacd.neighbour3 = pabd.key;
	pacd.neighbour4 = pbcd.key;
	addTetra( pacd );

	// update neightbours of new tetra
	pbcd.neighbour4 = pacd.key;
	updateTetra(pbcd);

	pabd.neighbour3 = pbcd.key;
	pabd.neighbour4 = pacd.key;
	updateTetra(pabd);

	//done with new tetras

	//now update the neighbours of old tetras
	Tetrahedron n;

	// start with neighbours of t
	if( getTetra( getNeighborByVertices( t, a, b, p ), n ) ) {
		replaceNeighbour( n, t.key, pabd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( t, a, c, p ), n ) ) {
		replaceNeighbour( n, t.key, pacd.key );
		updateTetra(n);
	}

	if( getTetra( getNeighborByVertices( t, b, c, p ), n ) ) {
		replaceNeighbour( n, t.key, pbcd.key );
		updateTetra(n);
	}

	// then neighbours of ta
	if( getTetra( getNeighborByVertices( ta, a, b, d ), n ) ) {
		replaceNeighbour( n, ta.key, pabd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( ta, a, c, d ), n ) ) {
		replaceNeighbour( n, ta.key, pacd.key );
		updateTetra(n);
	}
	if( getTetra( getNeighborByVertices( ta, b, c, d ), n ) ) {
		replaceNeighbour( n, ta.key, pbcd.key );
		updateTetra(n);
	}

	// done with updating neighbours
	// delete t and ta

	deleteTetra(t.key);
	deleteTetra(ta.key);
	
	// push tetrahedra pabd, pbcd, pacd on stack
	flipStack.push( pabd.key );
	flipStack.push( pbcd.key );
	flipStack.push( pacd.key );

	return true;
}

//assistant
void VoronoiShatter::replaceNeighbour( Tetrahedron &n, int oldkey, int newkey )
{
	if( n.neighbour1 == oldkey ) n.neighbour1 = newkey;
	else if( n.neighbour2 == oldkey ) n.neighbour2 = newkey;
	else if( n.neighbour3 == oldkey ) n.neighbour3 = newkey;
	else if( n.neighbour4 == oldkey ) n.neighbour4 = newkey;
}

int VoronoiShatter::getd( Tetrahedron &t, MPoint a, MPoint b, MPoint c )
{
	bool v1, v2, v3, v4;

	v1 = (t.v1.point == a ) || (t.v1.point == b ) || (t.v1.point == c );
	v2 = (t.v2.point == a ) || (t.v2.point == b ) || (t.v2.point == c );
	v3 = (t.v3.point == a ) || (t.v3.point == b ) || (t.v3.point == c );
	v4 = (t.v4.point == a ) || (t.v4.point == b ) || (t.v4.point == c );

	if( !v1 ) return 1;
	else if( !v2 ) return 2;
	else if( !v3 ) return 3;
	else if( !v4 ) return 4;
	else return -1;
}
