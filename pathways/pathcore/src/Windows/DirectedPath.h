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
#ifndef _DIRECTEDPATH_H_
#define _DIRECTEDPATH_H_

#include <math.h>

#include <vector>
#include <fstream>
#include <algorithm>

#include "boost/format.hpp"

using boost::format;


namespace asu_emit_qyan
{	
	class CDirectedPath  
	{		
	public:
		CDirectedPath(){};
		CDirectedPath(int pId, double pCost, const std::vector<int>& pVertexList)
			:m_nId(pId), m_dCost(pCost)
		{
			m_vVertexList.assign(pVertexList.begin(), pVertexList.end());
		}
		
		virtual ~CDirectedPath(){};
		
		// Getter and Setter
		int GetId() const { return m_nId; }
		void SetId(int val) { m_nId = val; }
		
		double GetCost() const { return m_dCost; }
		void SetCost(double val) { m_dCost = val; }
		
		int GetLength() const { return m_vVertexList.size(); }
		
		std::vector<int> GetVertexList() const { return m_vVertexList; }
		void SetVertexList(std::vector<int> val) { m_vVertexList = val; }
		
		int GetSourceNodeId() const { return m_nSourceNodeId; }
		void SetSourceNodeId(int val) { m_nSourceNodeId = val; }
		
		int GetTerminalNodeId() const { return m_nTerminalNodeId; }
		void SetTerminalNodeId(int val) { m_nTerminalNodeId = val; }
		
		// display the content
		void PrintOut(int iPath, std::ostream& out_stream) const
		{
                        /*
			out_stream << "Path: " << iPath 
                        <<" Coupling: " << format("%10.5e")%pow(10.0, -m_dCost) 
                        << " Length: " 
                        << m_vVertexList.size() << std::endl;
                        */
			out_stream << format(" %10.5e ")%pow(10.0, -m_dCost);
			//std::copy(m_vVertexList.rbegin(), m_vVertexList.rend(), std::ostream_iterator<int>(out_stream, " "));
                        for (int i=0; i<m_vVertexList.size(); i++) {
			    out_stream << m_vVertexList[i]+1 << " ";
                        }
			out_stream << std::endl; 
			//out_stream << std::endl <<  "*********************************************" << std::endl;	
		}


	private: // members
		int m_nId;
		int m_nLength;
		double m_dCost;  
		std::vector<int> m_vVertexList; 
		
		// intermediate variables
		int m_nSourceNodeId;
		int m_nTerminalNodeId;

		

	public:
		//// Comparator for paths: the smaller path has less cost.
		class Comparator 
		{
		public:
			// Lesson: the condition must be checked completely!!!
			bool operator() (const CDirectedPath& s1, const CDirectedPath& s2) const 
			{
				if (s1.GetCost() == s2.GetCost())
				{
					return s1.GetLength() <= s2.GetLength(); // only considering '<' will cause problems!!!
				}
				return s1.GetCost() < s2.GetCost();
			}

			//
			bool operator() (const CDirectedPath* s1, const CDirectedPath* s2) const 
			{
				return operator()(*s1, *s2);
			}
		}; 
	};
}

#endif //_DIRECTEDPATH_H_
