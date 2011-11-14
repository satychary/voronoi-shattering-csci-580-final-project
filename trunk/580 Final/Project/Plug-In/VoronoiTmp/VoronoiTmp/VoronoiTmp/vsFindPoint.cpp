//======================================================= 
// File: vsFindPoint.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"

//	Description:
//
//	Arguments:One MPoint and One TetraMap and One start key of the TetraMap
//
//	Return Value: one key indicate where the point locate
bool VoronoiShatter::findPoint(MPoint P, Tetrahedron &tetra){
	int current;
	current= tetra.key;
	if(!getTetra(current, tetra))
	{
		cout<<"Can't retreive the tetrahedron at vsFindPoint";
	}

	bool break1=false;
	int beginfacet=1;
	int result;
	MPoint temp_p;
	while(break1)
	{
		result=0;
		switch (beginfacet)
		{
			case 1:
				{					
					temp_p=tetra.v4.point;
					result=orient(tetra.v1,tetra.v2,tetra.v3,P)*orient(tetra.v1,tetra.v2,tetra.v3,temp_p);
					if(result<0)
					{
						// change neighbor and change the vertex, then continue
						findNeighbor(tetra.v1,tetra.v2,tetra.v3,tetra);
						break;
					}
				}
			case 2:
				{
					temp_p=tetra.v3.point;
					result=orient(tetra.v1,tetra.v2,tetra.v4,P)*orient(tetra.v1,tetra.v2,tetra.v4,temp_p);
					if(result<0)
					{
						findNeighbor(tetra.v1,tetra.v2,tetra.v4,tetra);
						// change neighbor and change the vertex, then continue
						break;
					}
				}
			case 3:
				{
					temp_p=tetra.v2.point;
					result=orient(tetra.v1,tetra.v3,tetra.v4,P)*orient(tetra.v1,tetra.v3,tetra.v4,temp_p);
					if(result<0)
					{
						findNeighbor(tetra.v1,tetra.v3,tetra.v4,tetra);
						// change neighbor and change the vertex, then continue
						break;
					}
				}
			case 4:
				{
					temp_p=tetra.v1.point;
					result=orient(tetra.v2,tetra.v3,tetra.v4,P)*orient(tetra.v2,tetra.v3,tetra.v4,temp_p);
					if(result<0)
					{
						findNeighbor(tetra.v2,tetra.v3,tetra.v4,tetra);
						// change neighbor and change the vertex, then continue
						break;
					}
				}
			default:
				{
					break1=true;
					break;
				}
		}
	}

	return break1;
}
void VoronoiShatter::findNeighbor(Vertex v1, Vertex v2, Vertex v3, Tetrahedron &tetra)
{
	int orign= tetra.key;
	Tetrahedron temp_tetra;
	int start=1;
	int equal_num=0;
	switch (start)
	{
		case 1:
				getTetra(tetra.neighbour1,temp_tetra);
				if(temp_tetra.v1.point==tetra.v1.point || temp_tetra.v1.point==tetra.v2.point ||temp_tetra.v1.point==tetra.v3.point || temp_tetra.v1.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v2.point==tetra.v1.point || temp_tetra.v2.point==tetra.v2.point ||temp_tetra.v2.point==tetra.v3.point || temp_tetra.v2.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v3.point==tetra.v1.point || temp_tetra.v3.point==tetra.v2.point ||temp_tetra.v3.point==tetra.v3.point || temp_tetra.v3.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v4.point==tetra.v1.point || temp_tetra.v4.point==tetra.v2.point ||temp_tetra.v4.point==tetra.v3.point || temp_tetra.v4.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(equal_num>=3)
				{
					getTetra(temp_tetra.key,tetra);
					break;
				}
		case 2:
				getTetra(tetra.neighbour2,temp_tetra);
				if(temp_tetra.v1.point==tetra.v1.point || temp_tetra.v1.point==tetra.v2.point ||temp_tetra.v1.point==tetra.v3.point || temp_tetra.v1.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v2.point==tetra.v1.point || temp_tetra.v2.point==tetra.v2.point ||temp_tetra.v2.point==tetra.v3.point || temp_tetra.v2.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v3.point==tetra.v1.point || temp_tetra.v3.point==tetra.v2.point ||temp_tetra.v3.point==tetra.v3.point || temp_tetra.v3.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v4.point==tetra.v1.point || temp_tetra.v4.point==tetra.v2.point ||temp_tetra.v4.point==tetra.v3.point || temp_tetra.v4.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(equal_num>=3)
				{
					getTetra(temp_tetra.key,tetra);
					break;
				}
		case 3:
				getTetra(tetra.neighbour3,temp_tetra);
				if(temp_tetra.v1.point==tetra.v1.point || temp_tetra.v1.point==tetra.v2.point ||temp_tetra.v1.point==tetra.v3.point || temp_tetra.v1.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v2.point==tetra.v1.point || temp_tetra.v2.point==tetra.v2.point ||temp_tetra.v2.point==tetra.v3.point || temp_tetra.v2.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v3.point==tetra.v1.point || temp_tetra.v3.point==tetra.v2.point ||temp_tetra.v3.point==tetra.v3.point || temp_tetra.v3.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v4.point==tetra.v1.point || temp_tetra.v4.point==tetra.v2.point ||temp_tetra.v4.point==tetra.v3.point || temp_tetra.v4.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(equal_num>=3)
				{
					getTetra(temp_tetra.key,tetra);
					break;
				}
		case 4:
				getTetra(tetra.neighbour4,temp_tetra);
				if(temp_tetra.v1.point==tetra.v1.point || temp_tetra.v1.point==tetra.v2.point ||temp_tetra.v1.point==tetra.v3.point || temp_tetra.v1.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v2.point==tetra.v1.point || temp_tetra.v2.point==tetra.v2.point ||temp_tetra.v2.point==tetra.v3.point || temp_tetra.v2.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v3.point==tetra.v1.point || temp_tetra.v3.point==tetra.v2.point ||temp_tetra.v3.point==tetra.v3.point || temp_tetra.v3.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(temp_tetra.v4.point==tetra.v1.point || temp_tetra.v4.point==tetra.v2.point ||temp_tetra.v4.point==tetra.v3.point || temp_tetra.v4.point==tetra.v4.point)
				{
					equal_num++;
				}
				if(equal_num>=3)
				{
					getTetra(temp_tetra.key,tetra);
					break;
				}
	}
	return ;
}