//======================================================= 
// File: vsGetDTformDT.cpp
//
// Authors: 
//=========================================================

#include "voronoiShatter.h"
#include <iostream>
#include <stdio.h>

//	Description:
//
//	Arguments:
//
//	Return Value;

void equation2( double *x, double *y, double a1, double b1, double c1, double a2, double b2, double c2 );
void equation3(double *x, double *y, double *z, double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double a3, double b3, double c3, double d3);
//double TTDet(double a, double b, double c, double d, double e, double f, double g, double h, double i);

void VoronoiShatter::getVDFormDT(){

	//VD's vertex:  DT's tetra, center of the sphere
	//VD's edge:  2 VD's vertex with the same DT's face
	//VD's face:  tetra which share DT's edge
	//VD's poly:  share DT's vertex
	std::cerr<<"START!!!!!!!!!!!!!!11: "<< std::endl;	
	int counter = 0;
	int currentKey = 0;
	int flag = 0;
	int first_vertex_flag = 0;
			int previousTetraKey = 0;
			int nextTetraKey = 0;
			int firstTetraKey = 0;
			int nowVertexId = 0;
			bool still_has_edge=true;
	Tetrahedron t;
	//VoronoiShatter voronoiShatter;
	//TetraMap pool = voronoiShatter.getPool();
	TetraMap pool = tetraPool;
	TetraMapItr itr = pool.begin();

	for( ; itr!=pool.end(); itr++){
		// get tetrahedront
		t = itr->second;
	
		// put all verteces to set, not include big tetra's vertecies
		if( t.v1 != bigTetra .v1 && t.v1 != bigTetra .v2 && t.v1 !=bigTetra .v3 && t.v1 != bigTetra .v4)
			vertexSet.insert(t.v1);
		if( t.v2 != bigTetra .v1 && t.v2 != bigTetra .v2 && t.v2 !=bigTetra .v3 && t.v2 != bigTetra .v4)
			vertexSet.insert(t.v2);
		if( t.v3 != bigTetra .v1 && t.v3 != bigTetra .v2 && t.v3 !=bigTetra .v3 && t.v3 != bigTetra .v4)
			vertexSet.insert(t.v3);
		if( t.v4 != bigTetra .v1 && t.v4 != bigTetra .v2 && t.v4 !=bigTetra .v3 && t.v4 != bigTetra .v4)
			vertexSet.insert(t.v4);
	}


	Tetrahedron originalTetra;

	
	while( !vertexSet.empty() )
	{
		first_vertex_flag = 0;
		// not sure
		// choose one vertex p
		checkVertex = vertexSet.begin();	

		// if no tetra contains p
		if( getTetra(checkVertex->incidentTetra, originalTetra)  ==false){
			vertexSet.erase(checkVertex);
			continue;
		}
			
		// tetra contains p
		inSetEdge e1;
		inSetEdge e2;
		inSetEdge e3;

		// push the edge to stack
		if(*checkVertex == originalTetra.v1){
			std::cerr<<"AAA: "<< std::endl;	
			e1.startVertex = originalTetra.v1;
			e1.endVertex = originalTetra.v2;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v1;
			e2.endVertex = originalTetra.v3;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v1;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeSet.insert(e1);
			edgeSet.insert(e2);
			edgeSet.insert(e3);
		}
		else if(*checkVertex == originalTetra.v2){
			std::cerr<<"BBB: "<< std::endl;	
			e1.startVertex = originalTetra.v2;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v2;
			e2.endVertex = originalTetra.v3;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v2;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeSet.insert(e1);
			edgeSet.insert(e2);
			edgeSet.insert(e3);
		}
		else if(*checkVertex == originalTetra.v3){
			std::cerr<<"CCC: "<< std::endl;	
			e1.startVertex = originalTetra.v3;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;

			e2.startVertex = originalTetra.v3;
			e2.endVertex = originalTetra.v2;
			e2.key = originalTetra.key;

			e3.startVertex = originalTetra.v3;
			e3.endVertex = originalTetra.v4;
			e3.key = originalTetra.key;

			edgeSet.insert(e1);
			edgeSet.insert(e2);
			edgeSet.insert(e3);
		}
		else if(*checkVertex == originalTetra.v4){
			std::cerr<<"DDD: "<< std::endl;	
			e1.startVertex = originalTetra.v4;
			e1.endVertex = originalTetra.v1;
			e1.key = originalTetra.key;
			std::cerr<<"DDD - e1.key: "<<e1.key<< std::endl;	

			e2.startVertex = originalTetra.v4;
			e2.endVertex = originalTetra.v2;
			e2.key = originalTetra.key;
			std::cerr<<"DDD - e2.key: "<<e2.key<< std::endl;	

			e3.startVertex = originalTetra.v4;
			e3.endVertex = originalTetra.v3;
			e3.key = originalTetra.key;
			std::cerr<<"DDD - e3.key: "<<e3.key<< std::endl;	

			edgeSet.insert(e1);
			edgeSet.insert(e2);
			edgeSet.insert(e3);
		}


		int vdEdgeTag = 0;
		int vdFaceTag = 0;
		VDfaceIndex.push_back(0);
		VDpolyIndex.push_back(0);

		// choose first edge
	//	currentEdgeItr = edgeSet.begin();
		bool hasNeighbor =true;


		still_has_edge = true;
		flag=0;
		inSetEdge usedEdge;
		int cou=0;
		while( still_has_edge==true  )
	//	for( currentEdgeItr; currentEdgeItr!=edgeSet.end(); currentEdgeItr++ )  //while haven't choosen the last edge
	//	for( ; currentEdgeItr!=edgeSet.end(); currentEdgeItr++ )  //while haven't choosen the last edge
		{


	//		if(still_has_edge==false)
	//			break;
			//choose one edge, from first to compare used set
	if(flag==0 || flag==3 ){
			currentEdgeItr = edgeSet.begin();			
			usedEdge.startVertex = currentEdgeItr->startVertex;
			usedEdge.endVertex = currentEdgeItr->endVertex;
			usedEdge.key = currentEdgeItr->key;
	
	}
	
	if(flag!=1 && flag!=4)
	{
		if(usedEdgeSet.size()!=edgeSet.size()-1)
		{

			while( usedEdgeSet.count(usedEdge)!=0   )
			{		
					currentEdgeItr++;
					usedEdge.startVertex = currentEdgeItr->startVertex;
					usedEdge.endVertex = currentEdgeItr->endVertex;
					usedEdge.key = currentEdgeItr->key;
			}
		}
		else
		{
					usedEdge.startVertex = currentEdgeItr->startVertex;
					usedEdge.endVertex = currentEdgeItr->endVertex;
					usedEdge.key = currentEdgeItr->key;
					flag=4;
					cou=0;
								previousTetraKey = 0;
			nextTetraKey = 0;
			firstTetraKey = 0;
			
			
			// find the edge from which tetra
			currentKey = currentEdgeItr->key; 
		first_vertex_flag = 0;
			firstTetraKey = currentEdgeItr->key;
		}
				
	}
		

			usedEdgeSet.insert(usedEdge);

			std::cerr<<"setEdgeSize: "<< edgeSet.size()<<std::endl;

			// choose one edge E
	//		inSetEdge currentEdge;
	//		currentEdge = edgeSet.top();
			hasNeighbor = true;
			
	
	

			if(flag!=1 && flag!=4){
			previousTetraKey = 0;
			nextTetraKey = 0;
			firstTetraKey = 0;
			
			
			// find the edge from which tetra
			currentKey = currentEdgeItr->key; 
	//		first_vertex_flag = 0;
			firstTetraKey = currentEdgeItr->key;
			}

			Tetrahedron currentTetra;
						
			while( hasNeighbor )
			{

						counter++;
				std::cerr<<"counter: "<< counter<<std::endl;	
				std::cerr<<"currentKey - StartAAA: "<< currentKey<<std::endl;	
				// get current tetra
				if( !getTetra( currentKey, currentTetra) ){
					//continue;
					break;
				}
				std::cerr<<"currentKey - StartBBB: "<< currentKey<<std::endl;	


						// tetra contains p
						inSetEdge ed1;
						inSetEdge ed2;
						inSetEdge ed3;
					// push the edge to stack
				if(*checkVertex == currentTetra.v1)
				{
					std::cerr<<"EEE: "<< std::endl;	
					ed1.startVertex = currentTetra.v1;
					ed1.endVertex = currentTetra.v2;
			//		findEdge  = edgeSet.find(ed1);
					
					if( edgeSet.count(ed1) == 0 )
					{
						ed1.key =currentTetra.key;
						edgeSet.insert(ed1);
					}

					ed2.startVertex = currentTetra.v1;
					ed2.endVertex = currentTetra.v3;
			//		findEdge  = edgeSet.find(ed2);
					
					if( edgeSet.count(ed2) == 0 )
					{
						ed2.key = currentTetra.key;
						edgeSet.insert(ed2);
					}

					ed3.startVertex = currentTetra.v1;
					ed3.endVertex = currentTetra.v4;
				//	findEdge  = edgeSet.find(ed3);
					
					if( edgeSet.count(ed3) == 0 )
					{
						ed3.key = currentTetra.key;
						edgeSet.insert(ed3);
					}
					
				}
				
				if(*checkVertex == currentTetra.v2)
				{
					std::cerr<<"FFF: "<< std::endl;	
					ed1.startVertex = currentTetra.v2;
					ed1.endVertex = currentTetra.v1;
	//				findEdge  = edgeSet.find(ed1);
					
					if( edgeSet.count(ed1) == 0 )
					{
						ed1.key =currentTetra.key;
						edgeSet.insert(ed1);
					}

					ed2.startVertex = currentTetra.v2;
					ed2.endVertex = currentTetra.v3;
		//			findEdge  = edgeSet.find(ed2);
					
					if( edgeSet.count(ed2) == 0 )
					{
						ed2.key = currentTetra.key;
						edgeSet.insert(ed2);
					}

					ed3.startVertex = currentTetra.v2;
					ed3.endVertex = currentTetra.v4;
				//	findEdge  = edgeSet.find(ed3);
					
					if( edgeSet.count(ed3) == 0 )
					{
						ed3.key = currentTetra.key;
						edgeSet.insert(ed3);
					}

	
				}
				
				if(*checkVertex == currentTetra.v3)
				{
					std::cerr<<"GGG: "<< std::endl;	
					ed1.startVertex = currentTetra.v3;
					ed1.endVertex = currentTetra.v1;
			//		findEdge  = edgeSet.find(ed1);
					
					if( edgeSet.count(ed1) == 0 )
					{
						ed1.key =currentTetra.key;
						edgeSet.insert(ed1);
					}

					ed2.startVertex = currentTetra.v3;
					ed2.endVertex = currentTetra.v2;
	//				findEdge  = edgeSet.find(ed2);
					
					if( edgeSet.count(ed2) == 0 )
					{
						ed2.key = currentTetra.key;
						edgeSet.insert(ed2);
					}

					ed3.startVertex = currentTetra.v3;
					ed3.endVertex = currentTetra.v4;
			//		findEdge  = edgeSet.find(ed3);
					
					if( edgeSet.count(ed3) == 0 )
					{
						ed3.key = currentTetra.key;
						edgeSet.insert(ed3);
					}

				}
				
				if(*checkVertex == currentTetra.v4)
				{
					std::cerr<<"HHH: "<< std::endl;	
					ed1.startVertex = currentTetra.v4;
					ed1.endVertex = currentTetra.v1;
		//			findEdge  = edgeSet.find(ed1);
					
					if( edgeSet.count(ed1) == 0 )
					{
						ed1.key =currentTetra.key;
						edgeSet.insert(ed1);
						std::cerr<<"HHH - ED1: "<<ed1.key<< std::endl;	
					}

					ed2.startVertex = currentTetra.v4;
					ed2.endVertex = currentTetra.v2;
	//				findEdge  = edgeSet.find(ed2);
					
					if( edgeSet.count(ed2) == 0 )
					{
						ed2.key = currentTetra.key;
						edgeSet.insert(ed2);
						std::cerr<<"HHH - ED2: "<<ed2.key<< std::endl;	
					}

					ed3.startVertex = currentTetra.v4;
					ed3.endVertex = currentTetra.v3;
			//		findEdge  = edgeSet.find(ed3);
					
					if( edgeSet.count(ed3) == 0 )
					{
						ed3.key = currentTetra.key;
						edgeSet.insert(ed3);
						std::cerr<<"HHH - ED3: "<<ed3.key<< std::endl;	
					}
	
				}

				


				// find VD's vertex vp of currentTetra
				Vertex vp;
				vp = findSphereCenter( currentTetra );
		
				// end of finding VD's vertex vp of currentTetra
				std::cerr<<"Vertex: "<<std::endl;	
				// put vp into VDvertex
				VDvertex.push_back(vp);
				// end of putting vp into VDvertex

				if(first_vertex_flag==0)
				{
					nowVertexId = VDvertex.size()-1;
					
				}
			

				if(flag==1 || (flag==4&&cou!=0)){
				// recode the VDedge
				if(VDvertex.size()>1)
				{
					Edge vdEdge;
					vdEdge.startVertexId = VDvertex.size() - 2;
					vdEdge.endVertexId = VDvertex.size() - 1;
					VDedge.push_back( vdEdge );
					std::cerr<<"Edge "<<std::endl;	
		
				// end record the VDedge
				}
				}
				// save now's tetra for preious
			

				// choose neighbor tetra which contain edge e
				Tetrahedron t1, t2, t3, t4;
				if( getTetra(currentTetra.neighbour1, t1) == false )
				{
			//		std::cerr<<"no neighborRRR"<<std::endl;
					
				}
				if( getTetra(currentTetra.neighbour2, t2) == false )
				{
	//				std::cerr<<"no neighbor2"<<std::endl;
					
				}
				if( getTetra(currentTetra.neighbour3, t3) == false )
				{
		//			std::cerr<<"no neighbor3"<<std::endl;
					
				}
				if( getTetra(currentTetra.neighbour4, t4) == false )
				{
		//			std::cerr<<"no neighbor4"<<std::endl;
					
				}

				
					// if tetra contains edge, and it is not the first tetra
					if ( checkEdge(t1, currentEdgeItr->startVertex, currentEdgeItr->endVertex)==true
						&& t1.key != previousTetraKey 
						&& t1.key != firstTetraKey
						){
								previousTetraKey = currentTetra.key;
						currentKey = t1.key;
						std::cerr<<"t1: "<<t1.key<<std::endl;
						std::cerr<<"currentKey - End - t1: "<< currentKey<<std::endl;	
								first_vertex_flag = 1;
						if(flag==4)
						{
							flag=4;
							cou=1;
						}
						else
							flag=1;
						break;
					}
					else if (checkEdge(t2, currentEdgeItr->startVertex, currentEdgeItr->endVertex) ==true
						&& t2.key != previousTetraKey
						&& t2.key != firstTetraKey
						){
								previousTetraKey = currentTetra.key;
						currentKey = t2.key;
						std::cerr<<"t2: "<<t2.key<<std::endl;	
						std::cerr<<"currentKey - End - t2: "<< currentKey<<std::endl;	
								first_vertex_flag = 1;
						if(flag==4)
						{
							flag=4;
							cou=1;
						}
						else
							flag=1;
						break;
					}
					else if (checkEdge(t3, currentEdgeItr->startVertex, currentEdgeItr->endVertex) ==true
						&& t3.key != previousTetraKey 
						&& t3.key != firstTetraKey
						){
								previousTetraKey = currentTetra.key;
						currentKey = t3.key;
						std::cerr<<"t3: "<<t3.key<<std::endl;	
						std::cerr<<"currentKey - End - t3: "<< currentKey<<std::endl;	
						std::cerr<<"previoueKey - End "<< previousTetraKey<<std::endl;	
								first_vertex_flag = 1;
						if(flag==4)
						{
							flag=4;
							cou=1;
						}
						else
							flag=1;
						break;
					}
					else if ( checkEdge(t4, currentEdgeItr->startVertex, currentEdgeItr->endVertex)  ==true
						&& t4.key != previousTetraKey 
						&& t4.key != firstTetraKey
						){
								previousTetraKey = currentTetra.key;
						currentKey = t4.key;
						std::cerr<<"t4: "<<t4.key<<std::endl;	
						std::cerr<<"currentKey - End - t4: "<< currentKey<<std::endl;	
								first_vertex_flag = 1;
						if(flag==4)
						{
							flag=4;
							cou=1;
						}
						else
							flag=1;
						break;
					}
					else{
						hasNeighbor = false;
						std::cerr<<"hereherehere"<<std::endl;
						if(flag==4)
							still_has_edge=false;

						flag = 2;
						//break;
					}
		
			}// end while of find neighbor
		if(flag==2  )
		{
	
				// first vertex and last vertex
				Edge vdEdge;
				vdEdge.startVertexId = VDvertex.size() - 1;
				vdEdge.endVertexId = nowVertexId;
				VDedge.push_back( vdEdge );
				first_vertex_flag = 0;
				std::cerr<<"First and Last Vertex"<<std::endl;	
		}
		if(flag==2 )
		{
				// this is a VDface
				for(int i=vdEdgeTag; i<VDedge.size(); i++)
				{
					VDface.push_back( i );
					std::cerr<<"Face: "<<VDedge.size()<<std::endl;	
				}

				vdEdgeTag = VDedge.size();
		//		if(vdEdgeTag!=0)
					VDfaceIndex.push_back( vdEdgeTag);
				// end of VDface
					flag = 3;
					
			}

		std::cerr<<"still_has_edge: "<<still_has_edge<<std::endl;	
		}//end while edge set
	


		//this is a VDpoly
	/*	for(int i=vdFaceTag; i<VDface.size(); i++)
		{
			VDpoly.push_back( i );
			std::cerr<<"Poly"<<std::endl;	
		}
		*/
		vdFaceTag = VDfaceIndex.size();
	//	if(vdFaceTag!=0)
			VDpolyIndex.push_back( vdFaceTag-1 );
		//end if VDpoly

		// erase the vertex
		vertexSet.erase( checkVertex );

		usedEdgeSet.clear();
		edgeSet.clear();
		flag=0;

	}//end while of vertex stk
		
	std::cerr<<"v1x: "<<VDvertex.at(0).point.x<<std::endl;
	std::cerr<<"v1x: "<<VDvertex.at(0).point.y<<std::endl;
	std::cerr<<"v1x: "<<VDvertex.at(0).point.z<<std::endl;

	std::cerr<<"v2x: "<<VDvertex.at(1).point.x<<std::endl;
	std::cerr<<"v2x: "<<VDvertex.at(1).point.y<<std::endl;
	std::cerr<<"v2x: "<<VDvertex.at(1).point.z<<std::endl;


	std::cerr<<"v3x: "<<VDvertex.at(2).point.x<<std::endl;
	std::cerr<<"v3x: "<<VDvertex.at(2).point.y<<std::endl;
	std::cerr<<"v3x: "<<VDvertex.at(2).point.z<<std::endl;


	std::cerr<<"v4x: "<<VDvertex.at(3).point.x<<std::endl;
	std::cerr<<"v4x: "<<VDvertex.at(3).point.y<<std::endl;
	std::cerr<<"v4x: "<<VDvertex.at(3).point.z<<std::endl;

	std::cerr<<"v5x: "<<VDvertex.at(4).point.x<<std::endl;
	std::cerr<<"v5x: "<<VDvertex.at(4).point.y<<std::endl;
	std::cerr<<"v5x: "<<VDvertex.at(4).point.z<<std::endl;

	std::cerr<<"v6x: "<<VDvertex.at(5).point.x<<std::endl;
	std::cerr<<"v6x: "<<VDvertex.at(5).point.y<<std::endl;
	std::cerr<<"v6x: "<<VDvertex.at(5).point.z<<std::endl;

	std::cerr<<"v7x: "<<VDvertex.at(6).point.x<<std::endl;
	std::cerr<<"v7x: "<<VDvertex.at(6).point.y<<std::endl;
	std::cerr<<"v7x: "<<VDvertex.at(6).point.z<<std::endl;

	std::cerr<<"v8x: "<<VDvertex.at(7).point.x<<std::endl;
	std::cerr<<"v8x: "<<VDvertex.at(7).point.y<<std::endl;
	std::cerr<<"v8x: "<<VDvertex.at(7).point.z<<std::endl;

	std::cerr<<"v9x: "<<VDvertex.at(8).point.x<<std::endl;
	std::cerr<<"v9x: "<<VDvertex.at(8).point.y<<std::endl;
	std::cerr<<"v9x: "<<VDvertex.at(8).point.z<<std::endl;

	std::cerr<<"v10x: "<<VDvertex.at(9).point.x<<std::endl;
	std::cerr<<"v10x: "<<VDvertex.at(9).point.y<<std::endl;
	std::cerr<<"v10x: "<<VDvertex.at(9).point.z<<std::endl;

	std::cerr<<"v11x: "<<VDvertex.at(10).point.x<<std::endl;
	std::cerr<<"v11x: "<<VDvertex.at(10).point.y<<std::endl;
	std::cerr<<"v11x: "<<VDvertex.at(10).point.z<<std::endl;

	std::cerr<<"v12x: "<<VDvertex.at(11).point.x<<std::endl;
	std::cerr<<"v12x: "<<VDvertex.at(11).point.y<<std::endl;
	std::cerr<<"v12x: "<<VDvertex.at(11).point.z<<std::endl;

	std::cerr<<"e1: "<<VDedge.at(0).startVertexId<<std::endl;
	std::cerr<<"e1: "<<VDedge.at(0).endVertexId<<std::endl;


		std::cerr<<"e2: "<<VDedge.at(1).startVertexId<<std::endl;
	std::cerr<<"e2: "<<VDedge.at(1).endVertexId<<std::endl;

		std::cerr<<"e3: "<<VDedge.at(2).startVertexId<<std::endl;
	std::cerr<<"e3: "<<VDedge.at(2).endVertexId<<std::endl;

		std::cerr<<"e4: "<<VDedge.at(3).startVertexId<<std::endl;
	std::cerr<<"e4: "<<VDedge.at(3).endVertexId<<std::endl;

		std::cerr<<"e5: "<<VDedge.at(4).startVertexId<<std::endl;
	std::cerr<<"e5: "<<VDedge.at(4).endVertexId<<std::endl;

		std::cerr<<"e6: "<<VDedge.at(5).startVertexId<<std::endl;
	std::cerr<<"e6: "<<VDedge.at(5).endVertexId<<std::endl;

	std::cerr<<"face1 "<<VDfaceIndex.at(0)<<std::endl;
	std::cerr<<"face2: "<<VDfaceIndex.at(1)<<std::endl;
	std::cerr<<"face3: "<<VDfaceIndex.at(2)<<std::endl;
	std::cerr<<"face4: "<<VDfaceIndex.at(3)<<std::endl;
		std::cerr<<"face5: "<<VDfaceIndex.at(4)<<std::endl;

	
			std::cerr<<"poly1 "<<VDpolyIndex.at(0)<<std::endl;
	std::cerr<<"poly2: "<<VDpolyIndex.at(1)<<std::endl;

	return;
}



bool VoronoiShatter::checkEdge(Tetrahedron t, Vertex start, Vertex end )
{
	Vertex p1, p2, p3, p4;
	p1 = t.v1;
	p2 = t.v2;
	p3 = t.v3;
	p4 = t.v4;

	if( ( p1 == start ) )
	{
		if( p2 == end || p3 == end || p4 == end )
			return true;
	}
	
	else if(( start == p2 ) )
	{
		if( end== p1 || end ==p3 || end ==p4 )
			return true;
	}
	
	else if(( start == p3 ) )
	{
	if( end == p1 || end==p2 || end ==p4 )
			return true;
	}
	
	else if( (start == p4)  )
	{
		if( end == p1 || end ==p2 || end ==p3 )
			return true;
	}
	else
	{
		return false;
	}
	return false;
}


void equation2(double *x, double *y, double a1, double b1, double c1, double a2, double b2, double c2)
{
*x = (c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1);
*y = (c1 - a1 * *x) / b1;
return;
}

void equation3(double *x, double *y, double *z, double a1, double b1, double c1, double d1, double a2, double b2, double c2, double d2, double a3, double b3, double c3, double d3)
{
equation2(x, y, a1 * c2 - a2 * c1, b1 * c2 - b2 * c1, d1 * c2 - d2 * c1, a1 * c3 - a3 * c1, b1 * c3 - b3 * c1, d1 * c3 - d3 * c1);
*z = (d1 - a1 * *x - b1 * *y) / c1;
return;
}
/*
double TTDet(double a, double b, double c, double, double d, double e, double f, double g, double h, double i)
{
	double det = 0;
	 
	det = a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h;
	return det;
}*/

Vertex VoronoiShatter::findSphereCenter( Tetrahedron t )
{
/*
//		a1*X + b1*Y + c1*Z = d1
//		a2*X + b2*Y + c2*Z = d2
//		a3*X + b3*Y + c3*Z = d3
//
//		now: center (a, b, c),  4 vertex (x1, y1, z1)    (x2, y2, z2)    (x3, y3, z3)    (x4, y4, z4)
//
//		(-2*x1 + 2*x2) * a  +  (-2*y1 + 2*y2) * b  +  (-2*z1 + 2*z2) * c = (x2)^2 - (x1)^2 + (y2)^2 - (y1)^2 + (z2)^2 - (z1)^2             use 1 = 2
//		(-2*x2 + 2*x3) * a  +  (-2*y2 + 2*y3) * b  +  (-2*z2 + 2*z3) * c = (x3)^2 - (x2)^2 + (y3)^2 - (y2)^2 + (z3)^2 - (z2)^2             use 2 = 3
//		(-2*x3 + 2*x4) * a  +  (-2*y3 + 2*y4) * b  +  (-2*z3 + 2*z4) * c = (x4)^2 - (x3)^2 + (y4)^2 - (y3)^2 + (z4)^2 - (z3)^2             use 3 = 4
//
//		X^2 - Y^2 = (X+Y) * (X-Y)
*/

	Vertex center;
	double x1, y1, z1;
	double x2, y2, z2;
	double x3, y3, z3;
	double x4, y4, z4;

	x1 = t.v1.point.x;
	y1 = t.v1.point.y;
	z1 = t.v1.point.z;

	x2 = t.v2.point.x;
	y2 = t.v2.point.y;
	z2 = t.v2.point.z;

	x3 = t.v3.point.x;
	y3 = t.v3.point.y;
	z3 = t.v3.point.z;

	x4 = t.v4.point.x;
	y4 = t.v4.point.y;
	z4 = t.v4.point.z;


	
	double x, y, z;
	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;

	a1 = ( -2*x1 + 2*x2 );
	b1 = ( -2*y1 + 2*y2 );
	c1 = ( -2*z1 + 2*z2 );
	d1 = ( x2 + x1 )*( x2 - x1 ) + ( y2 + y1 )*( y2 - y1 ) + ( z2 + z1 )*( z2 - z1 );

	a2 = ( -2*x2 + 2*x3 );
	b2 = ( -2*y2 + 2*y3 );
	c2 = ( -2*z2 + 2*z3 );
	d2 = ( x3 + x2 )*( x3 - x2 ) + ( y3 + y2 )*( y3 - y2 ) + ( z3 + z2 )*( z3 - z2 );

	a3 = ( -2*x3 + 2*x4 );
	b3 = ( -2*y3 + 2*y4 );
	c3 = ( -2*z3 + 2*z4 );
	d3 = ( x4 + x3 )*( x4 - x3 ) + ( y4 + y3 )*( y4 - y3 ) + ( z4 + z3 )*( z4 - z3 );

//	equation3(&x, &y, &z, a1, b1, c1, d1, a2, b2, c2, d2, a3, b3, c3, d3);
	


	double cram_x_up;
	double cram_y_up;
	double cram_z_up;
	double detA;
                              
	//detA = TTDet( a1, b1, c1, a2, b2, c2, a3, b3, c3 ) ;
	detA =	a1*b2*c3 + b1*c2*a3 + c1*a2*b3 - c1*b2*a3 - b1*a2*c3 - a1*c2*b3;
	
	                                            
	//cram_x_up = TTDet( d1, b1, c1, d2, b2, c2, d3, b3, c3 );
	cram_x_up = d1*b2*c3 + b1*c2*d3 + c1*d2*b3 - c1*b2*d3 - b1*d2*c3 - d1*c2*b3;

	    
	//cram_y_up = TTDet( a1, d1, c1, a2, d2, c2, a3, d3, c3 );
	cram_y_up = a1*d2*c3 + d1*c2*a3 + c1*a2*d3 - c1*d2*a3 - d1*a2*c3 - a1*c2*d3;

	    //                                       a   b     c     d    e    f     g   h      i
	//cram_z_up = TTDet( a1, b1, d1, a2, b2, d2, a3, b3, d3 );
	cram_z_up = a1*b2*d3 + b1*d2*a3 + d1*a2*b3 - d1*b2*a3 - b1*a2*d3 - a1*d2*b3;

	x = cram_x_up / detA;
	y = cram_y_up / detA;
	z = cram_z_up / detA;


	center.point.x = x;
	center.point.y = y;
	center.point.z = z;

	return center;
}

void VoronoiShatter::getPolyFace(int faceId, MPointArray &a)
{/*
	int startFaceId;
	int endFaceId;
	startFaceId = (int)VDpolyIndex.at(polyId);
	endFaceId = (int)VDpolyIndex.at(polyId+1);
	

	for(int i=startFaceId; i<endFaceId; i++)
{*/
		int startEdgeId, endEdgeId;
		startEdgeId = VDfaceIndex.at(faceId);
		endEdgeId = VDfaceIndex.at( faceId+1 );

		for(int j=startEdgeId; j<endEdgeId; j++)
		{
			int startVertexId, endVertexId;
			startVertexId = VDedge.at(j).startVertexId;
			//endVertexId = VDedge.at(j).endVertexId;
			a.append(VDvertex.at(startVertexId).point);
			//a.append(VDvertex.at(endVertexId).point);
		}

	//
		//a.remove(a.length()-1);
	return;
}
