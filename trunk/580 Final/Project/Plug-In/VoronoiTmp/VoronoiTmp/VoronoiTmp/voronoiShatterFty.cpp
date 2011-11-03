//====================================================
// File: voronoiShatterFty.cpp
//
// Node Factory: voronoiShatterFty
//
// Authors: 
//=====================================================

#include "voronoiShatterFty.h"

int voronoiShatterFty::meshCount = 0;

//  Description:
//		voronoiShatter constructor
voronoiShatterFty::voronoiShatterFty(void){
}

//	Description:
//		voronoiShatter destructor
voronoiShatterFty::~voronoiShatterFty(void){
}

// Description:
//		voronoiShatter setMesh
void voronoiShatterFty::setMesh(MObject mesh){
	
	fMesh = mesh;
}

// Description:
//		get static variable meshCount
int voronoiShatterFty::getMeshCount(){
	return meshCount;
}
