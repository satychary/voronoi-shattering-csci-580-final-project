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
		return false;
	}
	bool break1=false;
	int beginfacet=1;
	int result;
	bool no_neighbor1;
	bool no_neighbor2;
	bool no_neighbor3;
	bool no_neighbor4;
	bool final_result=false;
	MPoint temp_p;
	while(!break1)
	{
		result=0;
		no_neighbor1=false;
		no_neighbor2=false;
		no_neighbor3=false;
		no_neighbor4=false;
		switch (beginfacet)
		{
			case 1:
				{					
					temp_p=tetra.v4.point;
					result=orient(tetra.v1,tetra.v2,tetra.v3,P)*orient(tetra.v1,tetra.v2,tetra.v3,temp_p);
					if(result<0)
					{
						// change neighbor and change the vertex, then continue
						if(findNeighbor(tetra.v1,tetra.v2,tetra.v3,tetra))
						{
							no_neighbor1=false;
							break;
						}
						else
						{
							no_neighbor1=true;
						}
					}
				}
			case 2:
				{
					temp_p=tetra.v3.point;
					result=orient(tetra.v1,tetra.v2,tetra.v4,P)*orient(tetra.v1,tetra.v2,tetra.v4,temp_p);
					if(result<0)
					{
						if(findNeighbor(tetra.v1,tetra.v2,tetra.v4,tetra))
						{
							no_neighbor2=false;
							break;
						}
						else
						{
							no_neighbor2=true;
						}
					}
				}
			case 3:
				{
					temp_p=tetra.v2.point;
					result=orient(tetra.v1,tetra.v3,tetra.v4,P)*orient(tetra.v1,tetra.v3,tetra.v4,temp_p);
					if(result<0)
					{
						if(findNeighbor(tetra.v1,tetra.v3,tetra.v4,tetra))
						{
							no_neighbor3=false;
							break;
						}
						else
						{
							no_neighbor3=true;
						}
					}
				}
			case 4:
				{
					temp_p=tetra.v1.point;
					result=orient(tetra.v2,tetra.v3,tetra.v4,P)*orient(tetra.v2,tetra.v3,tetra.v4,temp_p);
					if(result<0)
					{
						if(findNeighbor(tetra.v2,tetra.v3,tetra.v4,tetra))
						{
							no_neighbor4=false;
							break;
						}
						else
						{
							no_neighbor4=true;
						}
					}
				}
			default:
				{
					if(no_neighbor1&&no_neighbor2&&no_neighbor3&&no_neighbor4)
					{
						final_result=false;
						break1=true;
						break;
					}
					else
					{
						final_result=true;
						break1=true;						
						break;
					}
				}
		}
	}
	return final_result;
}
bool VoronoiShatter::findNeighbor(Vertex v1, Vertex v2, Vertex v3, Tetrahedron &tetra)
{
	int orign= tetra.key;
	Tetrahedron temp_tetra;
	int start=1;
	int equal_num=0;
	bool break2=false;
	bool findNeighbor_result=false;
		switch (start)
		{
		case 1:
				equal_num=0;
				if(getTetra(tetra.neighbour1,temp_tetra))
				{
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
						findNeighbor_result=true;
						break;
					}
				}
		case 2:
				equal_num=0;
				if(getTetra(tetra.neighbour2,temp_tetra))
				{
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
						findNeighbor_result=true;
						break;
					}
				}
				
		case 3:
				equal_num=0;
				if(getTetra(tetra.neighbour3,temp_tetra))
				{
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
						findNeighbor_result=true;
						break;
					}
				}
			
		case 4:
				equal_num=0;
				if(getTetra(tetra.neighbour4,temp_tetra))
				{
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
						findNeighbor_result=true;
						break;
					}
				}
		default:
			findNeighbor_result=false;
			break;
		}
	return findNeighbor_result;
}