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


#ifndef _KSHORTESTPATHS_H_
#define _KSHORTESTPATHS_H_

#include "ShortestPath.h"

namespace asu_emit_qyan
{
	using namespace std;

	class CKShortestPaths  
	{
	public:
		CKShortestPaths(const CDirectedGraph& rGraph, int nSource, int nTerminal, int nTopk);
		virtual ~CKShortestPaths();

		vector<CDirectedPath*> GetTopKShortestPaths();

	private: // methods

		void _Init();
		void _SearchTopKShortestPaths();

		void _DetermineCost2Target(vector<int> vertices_list, int deviated_node_id);
		void _RestoreEdges4CostAjustment(vector<int> vertices_list, int start_node_id, int end_node_id, bool is_deviated_node = false);
		void _UpdateWeight4CostUntilNode(int node_id);
		void _ReverseEdgesInGraph(CDirectedGraph& g);
		bool _EdgeHasBeenUsed(int start_node_id, int end_node_id);

	private: // members
		
		int m_nTopK;
		int m_nSourceNodeId;
		int m_nTargetNodeId;
		
		const CDirectedGraph& m_rGraph;
		CDirectedGraph* m_pIntermediateGraph;
		CShortestPath* m_pShortestPath4IntermediateGraph;

		// variable to store the top shortest paths
		vector<CDirectedPath*> m_vTopShortestPaths;

		// a queue of candidates
		set<CDirectedPath*, CDirectedPath::Comparator> m_candidatePathsSet;  

		// index for node where the path derives from others
		map<int, int> m_pathDeviatedNodeMap;
	}; 
}

#endif //_KSHORTESTPATHS_H_

