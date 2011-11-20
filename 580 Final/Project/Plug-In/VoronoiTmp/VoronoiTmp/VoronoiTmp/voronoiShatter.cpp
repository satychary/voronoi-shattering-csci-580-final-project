#include "voronoiShatter.h"


VoronoiShatter::VoronoiShatter(void)
{
	currentKey = 0;
}


VoronoiShatter::~VoronoiShatter(void)
{
}



//	Description: 
//		get the tetrahedron form our pool by key
//	Arguments:
//		key   -- key for map retrieving
//		tetra -- referenced, returned as the tetra associated with key  
//	Return Value;
//      true  -- if success
//		false -- if key is not exist
bool VoronoiShatter::getTetra(int key, Tetrahedron &tetra)
{
	TetraMapItr result = tetraPool.find(key);
	if(result != tetraPool.end()){
		tetra = result->second;
		return true;
	}
	else
		return false;
}

//	Description:
//		add tetra to our pool
//	Arguments:
//		tetra -- tetra to be added
//	Return Value;
//		true  -- if success
//		false -- if there is already a tetra with same key, 
//               and our pool remain unchanged
bool VoronoiShatter::addTetra(Tetrahedron &tetra)
{
	// set currentKey
	tetra.key = currentKey;
	// set incident for vertices
	tetra.v1.incidentTetra = currentKey;
	tetra.v2.incidentTetra = currentKey;
	tetra.v3.incidentTetra = currentKey;
	tetra.v4.incidentTetra = currentKey;

	TetraMapItem newItem(currentKey, tetra);
	currentKey++;
	return tetraPool.insert(newItem).second;
}

//	Description:
//		delete a tetra from our pool by key
//	Arguments:
//		key -- key of tetra to delete
//	Return Value;
//		void
void VoronoiShatter::deleteTetra(int key)
{
	tetraPool.erase(key);
}

//	Description:
//		update tetra in our pool
//	Arguments:
//		tetra -- tetra to be updated
//	Return Value;
//		true  -- if success
//      false -- if tetra not found
bool VoronoiShatter::updateTetra(Tetrahedron &tetra)
{
	TetraMapItr newTetra = tetraPool.find(tetra.key);
	if(newTetra != tetraPool.end()){
		newTetra->second = tetra;
		return true;
	}
	else
		return false;
}

//	Description:
//		get the first tetra form our pool
//	Arguments:
//		tetra -- referenced, retruned as the first tetra
//	Return Value;
//		true  -- if success
//		false -- if our pool is currently empty
bool VoronoiShatter::firstTetra(Tetrahedron &tetra)
{
	TetraMapItr result= tetraPool.begin();
	if(result != tetraPool.end()){
		tetra = result->second;
		return true;
	}
	else
		return false;
}

//	Description:
//		get AND delete a tetra from our pool by key
//	Arguments:
//		key   -- key to retrieve
//		tetra -- referenced, returned as the tetra associated with key
//	Return Value;
bool VoronoiShatter::popTetra(int key, Tetrahedron &tetra)
{
	if(getTetra(key, tetra)){
		deleteTetra(key);
		return true;
	}
	else
		return false;
}

//	Description:
//		get the size of current pool
//	Arguments:
//		
//	Return Value;
int VoronoiShatter::getTetraNum()
{
	return tetraPool.size();
}

//	Description:
//		get current pool
//	Arguments:
//		
//	Return Value;
TetraMap VoronoiShatter::getPool()
{
	return tetraPool;
}

//	Description:
//		set bounding box
//	Arguments:
//		bbx -- bounding box to set
//	Return Value;
void VoronoiShatter::setBoundingBox(MBoundingBox bbx)
{
	boundingBox = bbx;
}

//	Description:
//		set transform matrix
//	Arguments:
//		mx -- transform matrix to set
//	Return Value;
void VoronoiShatter::setTransformMatrix(MMatrix mx)
{
	tMatrix = mx;
}