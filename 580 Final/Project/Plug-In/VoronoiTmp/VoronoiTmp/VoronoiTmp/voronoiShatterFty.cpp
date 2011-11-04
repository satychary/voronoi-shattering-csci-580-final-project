//====================================================
// File: voronoiShatterFty.cpp
//
// Node Factory: voronoiShatterFty
//
// Authors: 
//=====================================================

#include "voronoiShatterFty.h"

int VoronoiShatterFty::meshCount = 0;

//  Description:
//		voronoiShatter constructor
VoronoiShatterFty::VoronoiShatterFty(void){
}

//	Description:
//		voronoiShatter destructor
VoronoiShatterFty::~VoronoiShatterFty(void){
}

// Description:
//		voronoiShatter setMesh
void VoronoiShatterFty::setMesh(MObject mesh){
	
	fMesh = mesh;
}

// Description:
//		get static variable meshCount
int VoronoiShatterFty::getMeshCount(){
	return meshCount;
}
