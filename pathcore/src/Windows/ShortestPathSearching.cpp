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


#include <iostream>

#include "KShortestPaths.h"

using namespace std;
using namespace asu_emit_qyan;

int ShortestPathSearching(CConfigCenter::IntPair_Double_Map *m_pDirectedEdges, 
    int nVertex, int nSource, int nTarget, int nPath)
{
	CDirectedGraph dg(m_pDirectedEdges, nVertex);
	CKShortestPaths ksp(dg, nSource, nTarget, nPath);
	vector<CDirectedPath*> topK_shortest_paths = ksp.GetTopKShortestPaths();

	/* print all the paths */
        int i=0;
	for (vector<CDirectedPath*>::iterator pos=topK_shortest_paths.begin(); pos!=topK_shortest_paths.end(); ++pos)
	{
		i++;
		(*pos)->PrintOut(i, cout);
	}

	return 0;
}
