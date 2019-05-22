/***************************************************************************
 *   Copyright (C) 2008 by Xiangqian Hu xqhu@duke.edu                      *
 *   Based on the Yan Qi's K-shortest path code project at Google Code     *
 *   (http://code.google.com/p/k-shortest-paths), Boost libraries          *
 *   (http://www.boost.org), and Ilya Balabin's PDB class (pdh.h)          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
#include "ShortestPath.h"

namespace asu_emit_qyan
{
	using namespace std;
	using namespace boost;
	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////
	
	/************************************************************************/
	/* Default Constructor
	/************************************************************************/
	CShortestPath::CShortestPath( const CDirectedGraph& rGraph ) : m_rGraph(rGraph)
	{
		m_nSourceNodeId = -1; // the source node id is 0 by default.
		_Init();
	}
	
	CShortestPath::~CShortestPath()	{}
	

	/************************************************************************/
	/* Initiate members
	/************************************************************************/
	void CShortestPath::_Init()
	{
		int vertices_count = m_rGraph.GetNumberOfVertices();
		
		// First: edges with weights
		for (int i=0; i!=vertices_count; ++i)
		{
			for (int j=0; j!=vertices_count; ++j)
			{
				if (m_rGraph.GetWeight(i,j) != CDirectedGraph::DISCONNECT)
				{
					m_vEdges.push_back(Edge_Type(i,j));
					m_vWeights.push_back(m_rGraph.GetWeight(i,j));
				}
			}
		} 
	}

	/************************************************************************/
	/* Analysis of m_vResult4Vertices and m_vResult4Distance to generate the 
	/* shortest path.
	/************************************************************************/
	CDirectedPath* CShortestPath::_GetShortestPath( int nTargetNodeId )
	{
		vector<int> vertex_list;
		
		// Check the input
		if (nTargetNodeId >= m_rGraph.GetNumberOfVertices() || nTargetNodeId < 0)
		{
			cerr << "Please enter a right terminal id!" << std::endl;
			return new CDirectedPath(-1, CDirectedGraph::DISCONNECT, vertex_list);
		}
		
		if (m_distanceMap[nTargetNodeId] == CDirectedGraph::DISCONNECT)
		{
			cerr << "The path doesn't exist!" << std::endl;
			return new CDirectedPath(-2, CDirectedGraph::DISCONNECT, vertex_list);
		}
		
		// Determine the shortest path from the source to the terminal.	
		int cur_vertex = nTargetNodeId;
		list<int> tmp_list;
		tmp_list.push_front(nTargetNodeId);
		do 
		{
			if (m_nextNodeMap[cur_vertex] == m_nSourceNodeId)
			{
				if(cur_vertex != m_nSourceNodeId) tmp_list.push_front(m_nSourceNodeId);
				break;
			}else
			{
				cur_vertex = m_nextNodeMap[cur_vertex];
				tmp_list.push_front(cur_vertex);
			}
		} while(1);
		//
		copy(tmp_list.begin(), tmp_list.end(), back_inserter(vertex_list));
		
		// 
		return new CDirectedPath(0, m_distanceMap[nTargetNodeId], vertex_list);
	}

	/************************************************************************/
	/* Calculate the shortest path from a source to a target.
	/************************************************************************/
	CDirectedPath* CShortestPath::GetShortestPath( int nSourceNodeId, int nTargetNodeId )
	{
		if (m_nSourceNodeId != nSourceNodeId)
		{
			m_nSourceNodeId = nSourceNodeId;
			_DijkstraShortestPathsAlg();
		}
		
		return _GetShortestPath(nTargetNodeId);	
	}
	
	/************************************************************************/
	/* Based on the input - the source of the path, create a steiner tree. (???) 
	/************************************************************************/
	void CShortestPath::ConstructPathTree( int nSourceNodeId )
	{
		m_nSourceNodeId = nSourceNodeId;
		_DijkstraShortestPathsAlg();
	}

	/************************************************************************/
	/* 
	/************************************************************************/
	void CShortestPath::_DijkstraShortestPathsAlg()
	{
		int edges_count = m_rGraph.GetNumberOfEdges();
		int vertices_count = m_rGraph.GetNumberOfVertices();

		//////////////////////////////////////////////////////////////////////////
		// Initiate the boost graph
		vector<Vertex_Descriptor> vResult4Vertices;
		vector<double> vResult4Distance;
		Boost_Graph_Type g(vertices_count);
		property_map<Boost_Graph_Type, edge_weight_t>::type weightmap = get(edge_weight, g);
		//
		for (std::size_t j = 0; j < edges_count; ++j) 
		{
			Edge_Descriptor e; bool inserted;
			tie(e, inserted) = add_edge(m_vEdges[j].first, m_vEdges[j].second, g);
			weightmap[e] = m_vWeights[j];
		}		
		
		// about the vertices in the boost graph
		vResult4Vertices.resize(num_vertices(g));
		vResult4Distance.resize(num_vertices(g));
		Vertex_Descriptor s = vertex(m_nSourceNodeId, g);
			
		// run the algorithm
		// VC++ has trouble with the named parameters mechanism
		property_map<Boost_Graph_Type, vertex_index_t>::type indexmap = get(vertex_index, g);
		dijkstra_shortest_paths(g, s, &vResult4Vertices[0], &vResult4Distance[0], weightmap, indexmap, 
			std::less<double>(), closed_plus<double>(), 
			CDirectedGraph::DISCONNECT, 0, default_dijkstra_visitor());
		
		//////////////////////////////////////////////////////////////////////////
		// Set the results
		for (int i=0; i<vResult4Vertices.size(); ++i)
		{
			m_distanceMap[i] = vResult4Distance[i];
			m_nextNodeMap[i] = vResult4Vertices[i];
		}
		
	}
}
