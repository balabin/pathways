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
#ifndef _DIRECTEDGRAPH_H_
#define _DIRECTEDGRAPH_H_

#include "ConfigCenter.h"
#include <string>

namespace asu_emit_qyan
{
    // ____________________________________________________________________________
    //
    // Class:       CDirectedGraph
    //
    // Purpose:     CDirectedGraph defines the directed graph with a list of
    //              directed edges, associated with its weight.
    //
    // Notes:		Two ways to construct a graph: 
	//				1. Assign the path of the file recording the graph
	//				2. Transfer an existing object.
    //
    // See Also:    
    //
    // ____________________________________________________________________________
	class CDirectedGraph  
	{
	public:
		const static double DISCONNECT;
		const static int DEADEND;
		//
		CDirectedGraph(){_Init();}
		CDirectedGraph(CConfigCenter::IntPair_Double_Map *m_pDirectedEdgesInp, int nVertex);
		virtual ~CDirectedGraph();
		
		CDirectedGraph(const CDirectedGraph& rGraph);
		CDirectedGraph& operator=(const CDirectedGraph& rGraph);
		
		// The methods below play the persistent role. 
		void PrintOut(const std::string& out_file_name) const;
		void PrintOut(std::ostream& out_stream) const;
		void PrintOutAllEdges(std::ostream& os) const;
		

		void RemoveEdge(int i, int j);
		void AddEdge(int i, int j, double weight);

		// Getter and setter
		int GetNumberOfVertices() const { return m_nNumberOfVertices; }
		void SetNumberOfVertices(int val) { m_nNumberOfVertices = val; }
		
		int GetNumberOfEdges() const { return m_nNumberOfEdges; }
		void SetNumberOfEdges(int val) { m_nNumberOfEdges = val; }
		
		double GetMaxWeight() const { return m_dMaxWeight; }
		void SetMaxWeight(double val) { m_dMaxWeight = val; }
		
		double GetMinWeight() const { return m_dMinWeight; }
		void SetMinWeight(double val) { m_dMinWeight = val; }
		
		double GetWeight(int i, int j) const { return m_pDirectedEdges->count(std::pair<int, int>(i,j)) ? (*m_pDirectedEdges)[std::pair<int, int>(i,j)] : DISCONNECT; }
		void SetWeight(int i, int j, double val) { (*m_pDirectedEdges)[std::pair<int, int>(i,j)] = val; }
		
	private:
		CConfigCenter::IntPair_Double_Map* m_pDirectedEdges;	
		
		int m_nNumberOfVertices;
		int m_nNumberOfEdges;
		
		double m_dMaxWeight;
		double m_dMinWeight;
		
		void _Init();
		
	};
	
}

#endif //_DIRECTEDGRAPH_H_
