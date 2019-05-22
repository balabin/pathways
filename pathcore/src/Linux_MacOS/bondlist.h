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
#ifndef _BONDLIST_H_
#define _BONDLIST_H_

#include <boost/regex.hpp>
#include <boost/functional.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>

#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>

#include "pdb.h"

#include "ConfigCenter.h"

#ifndef DIST_CUTOFF
#define DIST_CUTOFF  6
#endif

using namespace std;
using namespace boost;

using namespace asu_emit_qyan;

// bond attributes 
typedef struct Bond {

   int id0;
   int id1;

   /*-1: no connection; 0: convalent; 1: h bond; 2: space */
   int cont;

   double r; 
   double e;
   double tda;

};

// bond list
typedef vector<Bond> BondList;

int readBondList(string &file, BondList &blist);

int CalCoupling(Pdb &mol, BondList &clist0, BondList &hlist0,
                BondList &slist0, BondList &alist0, double radcut_space,
                double *cb, double *hb, double *sb, double *ab);

int GenGraph(CConfigCenter::IntPair_Double_Map *m_pDirectedEdges, Pdb &mol, BondList &clist0, BondList &hlist0, 
             BondList &slist0, BondList &alist0, double *center, double radcut); 

int ShortestPathSearching(CConfigCenter::IntPair_Double_Map *m_pDirectedEdges, 
                          int nVertex, int nSource, int nTarget, int nPath);

inline int ij2index(int i, int j);

#endif

