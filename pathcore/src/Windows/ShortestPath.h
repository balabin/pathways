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

#ifndef _SHORTETSPATH_H_
#define _SHORTETSPATH_H_

#include <map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include "ConfigCenter.h"
#include "DirectedGraph.h"
#include "DirectedPath.h"

namespace asu_emit_qyan
{
	using namespace boost;
	
	class CShortestPath  
	{
		typedef CConfigCenter::Int_Pair Edge_Type;
		typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, double > > Boost_Graph_Type;
		typedef graph_traits < Boost_Graph_Type >::edge_descriptor Edge_Descriptor;
		typedef graph_traits < Boost_Graph_Type >::vertex_descriptor Vertex_Descriptor;
		
	public:
		CShortestPath(const CDirectedGraph& rGraph);
		virtual ~CShortestPath();

		CDirectedPath* GetShortestPath(int nSourceNodeId, int nTargetNodeId);
		void ConstructPathTree(int nSourceNodeId);

		double GetDistance(int i) { return m_distanceMap.count(i)? m_distanceMap[i] : CDirectedGraph::DISCONNECT; }
		void SetDistance(int i, double new_value){ m_distanceMap[i] = new_value; }

		int GetNextNodeId(int i) { return m_distanceMap.count(i)? m_nextNodeMap[i] : CDirectedGraph::DEADEND;}
		void SetNextNodeId(int i, int val) { m_nextNodeMap[i] = val; }

	private: // methods
		void _Init();
		void _DijkstraShortestPathsAlg();
		CDirectedPath* _GetShortestPath(int nTargetNodeId);

	private: // members
		std::vector<CConfigCenter::Int_Pair> m_vEdges;
		std::vector<double> m_vWeights;

		std::map<int, double> m_distanceMap;
		std::map<int, int> m_nextNodeMap;
		
		int m_nSourceNodeId;
		const CDirectedGraph& m_rGraph;
	};
}

#endif //_SHORTETSPATH_H_
