//======================================================= 
// File: vsInSphere.cpp
//
// Authors:  wch, 11/08/2011
//=========================================================

#include "voronoiShatter.h"
//#include <math.h>
//	Description:
//
//	Arguments:
//
//	Return Value;

// 4x4 determinate
double FFDet( double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz, double Px, double Py, double Pz ){
	
	//according to vsOrient.cpp
	double sum1=0, sum2=0, sum3=0, sum4=0, result=0;
	sum1 = - ( Bx*Cy*Pz - Bx*Py*Cz - Cx*By*Pz + Cx*Py*Bz + Px*By*Cz - Px*Cy*Bz );
	sum2 = ( Ax*Cy*Pz - Ax*Py*Cz - Cx*Ay*Pz + Cx*Py*Az + Px*Ay*Cz - Px*Cy*Az );
	sum3 =  - ( Ax*By*Pz - Ax*Py*Bz - Bx*Ay*Pz + Bx*Py*Az + Px*Ay*Bz - Px*By*Az );
    sum4 = ( Ax*By*Cz - Ax*Cy*Bz - Bx*Ay*Cz + Bx*Cy*Az + Cx*Ay*Bz - Cx*By*Az );

	result = sum1 + sum2 + sum3 + sum4;

	return result;
}


double VoronoiShatter::inSphere(Vertex *v1, Vertex *v2, Vertex *v3, Vertex *v4, MPoint p){

	/*
	/	5x5 determinant
	/
	/			Ax	Ay	Az	Ax^2+Ay^2+Az^2		1	
	/		|	Bx	By	Bz	Bx^2+By^2+Bz^2		1	|
	/		|	Cx	Cy	Cz	Cx^2+Cy^2+Cz^2		1	|
	/		|	Dx	Dy	Dz	Dx^2+Dy^2+Dz^2		1	|
	/			Px	Py	Pz	Px^2+Py^2+Pz^2		1
	/
	/		Ax^2+Ay^2+Az^2		call it (A)
	/		Bx^2+By^2+Bz^2		call it (B)
	/
	/											Bx/(B)		By/(B)		Bz/(B)		1												Ax/(A)	Ay/(A)	Az/(A)	1
	/		=		(B)(C)(D)(P)	|	Cx(C)		Cy/(C)	Cz/(C)		1	|		-		(A)(C)(D)(P)	|	Cx/(C)		Cy/(C)	Cz/(C)		1	|
	/										|	Dx/(D)	Dy/(D)	Dz/(D)	1	|										|	Dx/(D)	Dy/(D)	Dz/(D)	1	|
	/											Px/(P)		Py/(P)		Pz/(P)		1												Px/(P)		Py/(P)		PZ/(P)		1
	/
	/											Ax/(A)	Ay/(A)	Az/(A)	1												Ax/(A)	Ay/(A)	Az/(A)	1
	/		+		(A)(B)(D)(P)	|	Bx/(B)		By/(B)		Bz/(B)		1	|		-		(A)(B)(C)(P)	|	Bx/(B)		By/(B)		Bz/(B)		1	|
	/										|	Dx/(D)	Dy/(D)	Dz/(D)	1	|										|	Cx/(C)		Cy/(C)	Cz/(C)		1	|
	/											Px/(P)		Py/(P)		Pz/(P)		1												Px/(P)		Py/(P)		PZ/(P)		1
	/
	/
	/											Ax/(A)	Ay/(A)	Az/(A)	1	
	/		+		(A)(B)(C)(D)	|	Bx/(B)		By/(B)		Bz/(B)		1	|
	/										|	Cx(C)		Cy/(C)	Cz/(C)		1	|
	/											Dx/(D)	Dy/(D)	Dz/(D)	1	
	/
	*/



	double Ax = v1->point.x, Ay = v1->point.y, Az = v1->point.z;
	double Bx = v2->point.x, By = v2->point.y, Bz = v2->point.z;
	double Cx = v3->point.x, Cy = v3->point.y, Cz = v3->point.z;
	double Dx = v4->point.x, Dy = v4->point.y, Dz = v4->point.z;
	double Px = p.x, Py = p.y, Pz = p.z;

	double sqA =Ax * Ax + Ay * Ay + Az * Az;
	double sqB =Bx * Bx + By * By + Bz * Bz;
	double sqC =Cx * Cx + Cy * Cy + Cz * Cz;
	double sqD =Dx * Dx + Dy * Dy + Dz * Dz;
	double sqP =Px * Px + Py * Py + Pz * Pz;

	double sum1=0, sum2=0, sum3=0, sum4=0, sum5=0, result=0;


	sum1 = sqB * sqC * sqD * sqP * FFDet( Bx/sqB, By/sqB, Bz/sqB, Cx/sqC, Cy/sqC, Cz/sqC, Dx/sqD, Dy/sqD, Dz/sqD, Px/sqP, Py/sqP, Pz/sqP );
	sum2 = - ( sqA * sqC * sqD * sqP * FFDet( Ax/sqA, Ay/sqA, Az/sqA, Cx/sqC, Cy/sqC, Cz/sqC, Dx/sqD, Dy/sqD, Dz/sqD, Px/sqP, Py/sqP, Pz/sqP ) );
	sum3 = sqA * sqB * sqD * sqP * FFDet( Ax/sqA, Ay/sqA, Az/sqA, Bx/sqB, By/sqB, Bz/sqB, Dx/sqD, Dy/sqD, Dz/sqD, Px/sqP, Py/sqP, Pz/sqP );
	sum4 = - ( sqA * sqB * sqC * sqP * FFDet( Ax/sqA, Ay/sqA, Az/sqA, Bx/sqB, By/sqB, Bz/sqB, Cx/sqC, Cy/sqC, Cz/sqC, Px/sqP, Py/sqP, Pz/sqP ) );
	sum5 = sqA * sqB * sqC * sqD * FFDet( Ax/sqA, Ay/sqA, Az/sqA, Bx/sqB, By/sqB, Bz/sqB, Cx/sqC, Cy/sqC, Cz/sqC, Dx/sqD, Dy/sqD, Dz/sqD );

	result = sum1 + sum2 + sum3 + sum4 + sum5;

	return result;
}