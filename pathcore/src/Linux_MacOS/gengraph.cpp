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


#include "pdb.h"
#include "bondlist.h"

#include <boost/array.hpp>
#include "boost/multi_array.hpp"
#include <cassert>
#include <math.h>

#include "ConfigCenter.h"

// Generate graph using cut-off
int GenGraph(CConfigCenter::IntPair_Double_Map *m_pDirectedEdges,
             Pdb &mol, BondList &clist0, BondList &hlist0, BondList &slist0, 
             BondList &alist0, double *center, double radcut)
{

     Bond thisBond;

     double xa, ya, za;
     double r, tda;

     int id;

     // -1: outside the cut-off radius 0: inside
     vector<int>    iStatus;

     /* calculate distance matrix of PDB */

     /* initialize matrix */
     for (int i=0; i<mol.size(); i++) {
          iStatus.push_back(-1);
     }

     for (int i=0; i<mol.size(); i++) {
         xa=mol[i].x;
         ya=mol[i].y;
         za=mol[i].z;

         r =(xa-center[0])*(xa-center[0]);
         r+=(ya-center[1])*(ya-center[1]);
         r+=(za-center[2])*(za-center[2]);
         r = sqrt(r);

         //cout<< "r=" << r<< " i="<<i<<" c"<<radcut<<endl;

         if (r <= radcut) { 
             iStatus[i]=0;
         }

     }

     /* set coupling for AB */
     for (int i=0; i<alist0.size(); i++) {
          int ia=alist0[i].id0;
          int ib=alist0[i].id1;

          if (iStatus[ia] < 0 || iStatus[ib] < 0) {continue;}

          if (alist0[i].cont < 0) {continue;}

          tda=alist0[i].tda;

             //cout <<"AB: ia"<<ia<<" ib"<<ib<<" tda"<<alist0[0].tda<<endl;

          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ia,ib), tda));
          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ib,ia), tda));

     }
     /* set coupling for C */
     for (int i=0; i<clist0.size(); i++) {
          int ia=clist0[i].id0;
          int ib=clist0[i].id1;

          if (iStatus[ia] < 0 || iStatus[ib] < 0) {continue;}

          if (clist0[i].cont < 0) {continue;}

          //if (ia == 351 || ib ==351) {
             //cout <<"C: ia"<<ia<<" ib"<<ib<<" tda"<<clist0[0].tda<<endl;
          //}
          tda=clist0[i].tda;

          //cout <<" ia"<<ia<<" ib"<<ib<<" tda"<<tda<<endl;
          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ia,ib), tda));
          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ib,ia), tda));

     }

     /* set coupling for H */
     for (int i=0; i<hlist0.size(); i++) {
          int ia=hlist0[i].id0;
          int ib=hlist0[i].id1;

          if (iStatus[ia] < 0 || iStatus[ib] < 0) {continue;}

          if (hlist0[i].cont < 0) {continue;}

          tda=hlist0[i].tda;

          //cout <<"H: ia"<<ia<<" ib"<<ib<<" tda"<<tda<<endl;

          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ia,ib), tda));
          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ib,ia), tda));

     }

     /* set coupling for space */
     for (int i=0; i<slist0.size(); i++) {
          int ia=slist0[i].id0;
          int ib=slist0[i].id1;

          if (iStatus[ia] < 0 || iStatus[ib] < 0) {continue;}

          if (slist0[i].cont < 0) {continue;}

          //if (ia == 351 || ib ==351) {
             //cout <<"TS: ia "<<ia<<" ib "<<ib<<" tda "<<slist0[i].tda<<endl;
          //}
          tda=slist0[i].tda;

          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ia,ib), tda));
          m_pDirectedEdges->insert(pair<pair<int, int>, double>(pair<int, int>(ib,ia), tda));

     }

     iStatus.erase(iStatus.begin(), iStatus.end());

     return EXIT_SUCCESS;

}

