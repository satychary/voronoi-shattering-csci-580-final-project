//======================================================= 
// File: vsOrient.cpp
//
// Authors:  wch, 11/08/2011
//=========================================================

#include "voronoiShatter.h"

//	Description:
//
//	Arguments:
//
//	Return Value;
double VoronoiShatter::orient(Vertex *v1, Vertex *v2, Vertex *v3, MPoint p){

	/*
	/	4x4 determinant
	/			Ax	Ay	Az	1	
	/		|	Bx	By	Bz	1	|
	/		|	Cx	Cy	Cz	1	|
	/			Px	Py	Pz	1
	/
	/	- ( BxCyPz - BxPyCz - CxByPz + CxPyBz + PxByCz - PxCyBz )
	/	+( AxCyPz - AxPyCz - CxAyPz + CxPyAz + PxAyCz - PxCyAz )
	/	- ( AxByPz - AxPyBz - BxAyPz + BxPyAz + PxAyBz - PxByAz )
	/	+( AxByCz - AxCyBz - BxAyCz + BxCyAz + CxAyBz - CxByAz )
	*/

	double Ax = v1->point.x, Ay = v1->point.y, Az = v1->point.z;
	double Bx = v2->point.x, By = v2->point.y, Bz = v2->point.z;
	double Cx = v3->point.x, Cy = v3->point.y, Cz = v3->point.z;
	double Px = p.x, Py = p.y, Pz = p.z;

	double sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, result = 0;

	sum1 = - ( Bx*Cy*Pz - Bx*Py*Cz - Cx*By*Pz + Cx*Py*Bz + Px*By*Cz - Px*Cy*Bz );
	sum2 = ( Ax*Cy*Pz - Ax*Py*Cz - Cx*Ay*Pz + Cx*Py*Az + Px*Ay*Cz - Px*Cy*Az );
	sum3 =  - ( Ax*By*Pz - Ax*Py*Bz - Bx*Ay*Pz + Bx*Py*Az + Px*Ay*Bz - Px*By*Az );
    sum4 = ( Ax*By*Cz - Ax*Cy*Bz - Bx*Ay*Cz + Bx*Cy*Az + Cx*Ay*Bz - Cx*By*Az );

	result = sum1 + sum2 + sum3 + sum4;

	return result;
}