//======================================================= 
// File: voronoiShatter.h
//
// Authors: 
//=========================================================

#ifndef _voronoiShatter
#define _voronoiShatter

// Maya include

// Function Sets
//
#include <maya/MFnMesh.h>

// Iterators
//
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshEdge.h>

// General
//
#include <maya/MGlobal.h>
#include <maya/MPoint.h>
#include <maya/MBoundingBox.h>
#include <maya/MMatrix.h>
#include <maya/MDagPath.h>

// STL include
//
#include <map>
#include <stack>
#include <set>

#include "Globals.h"

//  struct 
struct Vector
{
	double x;
	double y;
	double z;
};

struct Line
{
	MPoint la;
	MPoint lb;
};

struct Plane
{
	MPoint pa;
	MPoint pb;
	MPoint pc;
};

class Vertex
{
public:
	MPoint point;
	int incidentTetra;

	Vertex(){ e = 0.00001f;}

	bool operator<(const Vertex& v) const{
		if(point.x<v.point.x)
			return true;
		else if(point.y<v.point.y)
			return true;
		else if(point.z<v.point.z)
			return true;
		else
			return false;
	}

	bool operator==(const Vertex& v) const{
		if(abs(point.x-v.point.x)<e &&
			abs(point.y - v.point.y)<e &&
			abs(point.z - v.point.z)<e)
			return true;
		else
			return false;
	}

	bool operator!=(const Vertex& v) const{
		if(abs(point.x-v.point.x)<e &&
			abs(point.y - v.point.y)<e &&
			abs(point.z - v.point.z)<e)
			return false;
		else
			return true;
	}

private:
	float e;  // threshold
};

/*struct Vertex{
	MPoint point;
    int incidentTetra;        // optional, key of incident tetrahedron
};*/

class Tetrahedron
{
public:
	Vertex v1;                // vertex
	Vertex v2;
	Vertex v3;
	Vertex v4;
	int neighbour1;           // key of neighbour
	int neighbour2;
	int neighbour3;
	int neighbour4;
	int key;                  // interger for map retrieving

	Tetrahedron()
	{
		neighbour1 = -1;
		neighbour2 = -1;
		neighbour3 = -1;
		neighbour4 = -1;
		key = -1;
	}
};

// type define
typedef std::map<int, Tetrahedron> TetraMap;                // tetrahedron map type
typedef std::pair<int, Tetrahedron> TetraMapItem;           // tetrahedron item type
typedef std::map<int, Tetrahedron>::iterator TetraMapItr;   // tetrahedron iterator type
typedef std::set<Vertex>       VertexSet;

class VoronoiShatter
{
public:
	VoronoiShatter(void);
	~VoronoiShatter(void);

	//  Public Method
	//
	//  To team members: 
	//		Change the return value and the arguments as u need!
	Tetrahedron initializeBigTetra();

	double orient(Vertex v1, Vertex v2, Vertex v3, MPoint p);             // R
	double inSphere(Vertex v1, Vertex v2, Vertex v3, Vertex v4, MPoint p);

	bool findPoint(MPoint P, Tetrahedron &tetra);          // N, using WALK algorithm
	bool findNeighbor(Vertex v1, Vertex v2, Vertex v3, Tetrahedron &tetra);
	VertexSet generatePoints(int total,int section);
	void insertOnePoint(MPoint p);     // B
	bool flip( int key, MPoint p );       

	void getVDFormDT();        // CZ

	// mesh functions
	void setMesh(MDagPath mesh);
	void splitMesh(MDagPath &dagPath, Plane plane); // split a mesh using a plane

	// Mian action
	void perform();

	// set bounding box
	void setBoundingBox(MBoundingBox bbx);

	// set transform matrix
	void setTransformMatrix(MMatrix mx);

	// get pool size
	int getTetraNum();

    TetraMap getPool();

private:

	// Private method
	
	// Wrapper method for map
	bool getTetra(int key, Tetrahedron &tetra);         /* get a tetrahefron into tetra from our pool by key 
														   return false if:
														   key not exist
														   */			
	bool addTetra(Tetrahedron &tetra);					/* add tetra to our pool,
														   return false if:
														   there is already a tetra with same key */
	void deleteTetra(int key);							// delete tetra from our pool by key
	bool updateTetra(Tetrahedron &tetra);                
	bool firstTetra(Tetrahedron &tetra);				/* get the first tetrahedron into tetra from our pool
														   return false if: our pool is empty*/


	bool popTetra(int key,Tetrahedron &tetra);           /* get tetrahedron into tetra AND delete it form our pool by key,
															return flase if:
											                key not exist */
	// assistant funs for splitMesh
	MPoint getLinePlaneIntersection(Line line, Plane plane);
	Vector crossProduct(Vector v1, Vector v2);

	//assistant functions in addPoint
	int getNeighborByVertices( Tetrahedron &t, MPoint a, MPoint b, MPoint c );
	bool isInTetrahedron( Tetrahedron &tetra, MPoint v );
	int getd( Tetrahedron &t, MPoint a, MPoint b, MPoint c );

	//assistant function in flip
	bool flip23( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	bool flip32( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	bool flip44( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	void replaceNeighbour( Tetrahedron &n, int oldkey, int newkey );

	// Private member variable
	std::stack<int> flipStack;

	TetraMap tetraPool;         /* our pool where we put all tetrahedrons avaliable now, 
	                               each associated with an unique interger key value */

	int currentKey;            // curent key value for new tehrahedron created
	MBoundingBox boundingBox;  // bounding box of the mesh 
	MMatrix      tMatrix;      // transform matrix
	MDagPath      vMesh;        // mesh to play with
};

#endif