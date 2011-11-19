//======================================================= 
// File: voronoiShatter.h
//
// Authors: 
//=========================================================

#ifndef _voronoiShatter
#define _voronoiShatter

// Maya include
#include <maya/MPoint.h>
#include <maya/MBoundingBox.h>
#include <maya/MMatrix.h>

// STL include
#include <map>
#include <stack>

//  struct 
struct Tetrahedron;

struct Vertex{
	MPoint point;
    int incidentTetra;        // optional, key of incident tetrahedron
};

struct Tetrahedron{
	Vertex v1;                // vertex
	Vertex v2;
	Vertex v3;
	Vertex v4;
	int neighbour1;           // key of neighbour
	int neighbour2;
	int neighbour3;
	int neighbour4;
	int key;                  // interger for map retrieving
};

// type define
typedef std::map<int, Tetrahedron> TetraMap;                // tetrahedron map type
typedef std::pair<int, Tetrahedron> TetraMapItem;           // tetrahedron item type
typedef std::map<int, Tetrahedron>::iterator TetraMapItr;   // tetrahedron iterator type

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
	void findNeighbor(Vertex v1, Vertex v2, Vertex v3, Tetrahedron &tetra);
	MPoint* generatePoints(int total);
	void insertOnePoint(MPoint p);     // B
	void flip( int key, MPoint p );       

	void getVDFormDT();        // CZ

	// Mian action
	void perform();

	// set bounding box
	void setBoundingBox(MBoundingBox bbx);

	// set transform matrix
	void setTransformMatrix(MMatrix mx);

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

	//assistant functions in addPoint
	int getNeighborByVertices( Tetrahedron &t, MPoint a, MPoint b, MPoint c );
	bool isInTetrahedron( Tetrahedron &tetra, MPoint v );
	int getd( Tetrahedron &t, MPoint a, MPoint b, MPoint c );

	//assistant function in flip
	void flip23( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	void flip32( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	void flip44( Tetrahedron t, Tetrahedron ta, MPoint a, MPoint b, MPoint c, MPoint d, MPoint p );
	void replaceNeighbour( Tetrahedron &n, int oldkey, int newkey );

	// Private member variable
	TetraMap tetraPool;         /* our pool where we put all tetrahedrons avaliable now, 
	                               each associated with an unique interger key value */

	std::stack<int> flipStack;

	int currentKey;            // curent key value for new tehrahedron created
	MBoundingBox boundingBox;  // bounding box of the mesh 
	MMatrix      tMatrix;      // transform matrix
};

#endif