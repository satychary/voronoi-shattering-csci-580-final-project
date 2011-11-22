//======================================================= 
// File: vsGetDTformDT.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//
//	Arguments:
//
//	Return Value;
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
	
		// put all verteces to set
		setVertex.insert(t.v1);
//		stkVertex.push_back(t.v1);	
//		stkVertex.push_back(t.v2);
		setVertex.insert(t.v2);
		setVertex.insert(t.v3);
		setVertex.insert(t.v4);
	}

	itr = pool.begin();
	while(!setVertex.empty()){
		// choose one vertex vv
		checkVertex = setVertex.begin();
		t = itr->second;

		// find the trtra which contains vv 
		while(t.v1 != checkVertex && t.v2 != checkVertex && t.v3 != checkVertex && t.v4 != checkVertex){
			itr++;
			t= itr ->second;
		}



	}

	return;
}