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
#include <limits>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "DirectedGraph.h"

namespace asu_emit_qyan
{
	using namespace std;

	const int CDirectedGraph::DEADEND = -1;
	const double CDirectedGraph::DISCONNECT = (numeric_limits<double>::max)();

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	CDirectedGraph::CDirectedGraph( CConfigCenter::IntPair_Double_Map *m_pDirectedEdgesInp, int nVertex) 
	{
		// Initiate the members of the class
		_Init();

		m_nNumberOfVertices=nVertex;

		m_nNumberOfEdges = m_pDirectedEdgesInp->size();

                double w;
                int i, j;

                CConfigCenter::IntPair_Double_Map_Iterator edge_pos;
                for (edge_pos=m_pDirectedEdgesInp->begin(); edge_pos!=m_pDirectedEdgesInp->end(); ++edge_pos)
                {
                        i= edge_pos->first.first ; 
                        j= edge_pos->first.second; 
                        w= edge_pos->second;

                        m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(i,j), w));
			
			//
			if (w > m_dMaxWeight)
			{
				m_dMaxWeight = w;
			}
			//
			if (w < m_dMinWeight)
			{
				m_dMinWeight = w;
			}
                }

                /*
		while(k < m_pDirectedEdgesInp->size())
		{
                        i=m_pDirectedEdgesInp[i].first.first;
                        j=m_pDirectedEdgesInp[i].first.second;
                        w=m_pDirectedEdgesInp[i].second;

                        m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(i,j), w));
			
			//
			if (w > m_dMaxWeight)
			{
				m_dMaxWeight = w;
			}
			//
			if (w < m_dMinWeight)
			{
				m_dMinWeight = w;
			}
                        k++;
		}	
                */
	}

	CDirectedGraph::CDirectedGraph( const CDirectedGraph& rGraph )
	{
		*this = rGraph;	
	}

	CDirectedGraph& CDirectedGraph::operator=( const CDirectedGraph& rGraph )
	{
		m_nNumberOfVertices = rGraph.m_nNumberOfVertices;
		m_nNumberOfEdges = rGraph.m_nNumberOfEdges;

		m_pDirectedEdges = new CConfigCenter::IntPair_Double_Map(*(rGraph.m_pDirectedEdges));
		
		return *this;
	}

	CDirectedGraph::~CDirectedGraph()
	{
		if (m_pDirectedEdges != NULL)
		{
			delete m_pDirectedEdges;
		}
	}

	void CDirectedGraph::_Init()
	{
		m_nNumberOfEdges = 0;
		m_dMaxWeight = 0;
		m_dMinWeight = DISCONNECT;
		m_pDirectedEdges = new CConfigCenter::IntPair_Double_Map();
	}


	void CDirectedGraph::PrintOut( std::ostream& out_stream ) const
	{
		out_stream << m_nNumberOfVertices << endl << endl;
		PrintOutAllEdges(out_stream);
	}

	void CDirectedGraph::PrintOut( const std::string& out_file_name ) const
	{
		ofstream ofs;
		ofs.open(out_file_name.c_str(), ios::out);
		if (!ofs)
		{
			cout << "The file " << out_file_name << " can't be opened!" << endl;
			exit(1);
		}

		PrintOut(ofs);

		ofs.close();
	}

	void CDirectedGraph::PrintOutAllEdges( std::ostream& os ) const
	{
		CConfigCenter::IntPair_Double_Map_Iterator edge_pos;
		for (edge_pos=m_pDirectedEdges->begin(); edge_pos!=m_pDirectedEdges->end(); ++edge_pos)
		{
			os << edge_pos->first.first << "	"; // start point of the edge
			os << edge_pos->first.second << "	"; // end point of the edge
			os << edge_pos->second << endl;
		}
	}

	void CDirectedGraph::RemoveEdge( int i, int j )
	{
		CConfigCenter::IntPair_Double_Map_Iterator pos = m_pDirectedEdges->find(pair<int, int>(i,j));
		if (pos != m_pDirectedEdges->end())
		{
			m_pDirectedEdges->erase(pos);
		}
	}

	void CDirectedGraph::AddEdge( int i, int j, double weight )
	{
		(*m_pDirectedEdges)[pair<int, int>(i,j)] = weight;	
	}

}
