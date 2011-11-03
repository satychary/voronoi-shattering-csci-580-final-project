//=============================================== 
// File: voronoiShatterMain.cpp
//
// Author:
//================================================

#include "voronoiShatterCmd.h"
#include "voronoiShatterNode.h"

#include <maya/MFnPlugin.h>

//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
MStatus initializePlugin( MObject obj ){ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");

	// helloWorld test
	status = plugin.registerCommand( "helloVoronoi", voronoiShatterCmd::creator );
	if ( !status )
		status.perror( "registerCommand failed");

	/***********************************************************************************
	status = plugin.registerCommand( "shatter", voronoiShatterCmd::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	status = plugin.registerNode( "voronoiShatterNode",
								  voronoiShatterNode::id,
								  voronoiShatterNode::creator,
								  voronoiShatterNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
	**************************************************************************************/
	return status;
}

MStatus uninitializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand( "helloVoronoi" );
	if ( !status )
		status.perror( "deregisterCommand failed" );
	/***************************************************************************************
	status = plugin.deregisterCommand( "voronoiShatter" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	status = plugin.deregisterNode( voronoiShatterNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	****************************************************************************************/
	return status;
}
