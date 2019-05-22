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

// Generate through-space list  and Calculate coupling 
// Save all information into the file.
// Science Vol. 252, 1991, 1285-1288
// cb: convalent paramter
//     default: cb[0]=0.60 cb[1]=0.00 cb[2]=0.00
// hb: h-bond paramter
//     default: hb[0]=0.36 hb[1]=1.70 hb[2]=2.80
// sb: space paramter
//     default: sb[0]=0.60 sb[1]=1.70 sb[2]=1.40
int CalCoupling(Pdb &mol, BondList &clist0, BondList &hlist0, 
                BondList &slist0, BondList &alist0, 
                double radcut_space,
                double *cb, double *hb, double *sb, double *ab)
{

     Bond thisBond;

     double xa, ya, za;
     double xb, yb, zb;
     double r;

     int id;

     // distance matrix
     vector<double> distM;
     // connection matrix
     vector<double> contM;

     /* calculate distance matrix of PDB */
     /*
     cout << "cb "<<cb[0]<<" "<<cb[1]<<" "<<cb[2]<<endl;
     cout << "hb "<<hb[0]<<" "<<hb[1]<<" "<<hb[2]<<endl;
     cout << "sb "<<sb[0]<<" "<<sb[1]<<" "<<sb[2]<<endl;
     */

     for (int i=0; i<mol.size(); i++) {
         xa=mol[i].x;
         ya=mol[i].y;
         za=mol[i].z;

         for (int j=0; j<=i; j++) {
             xb=mol[j].x;
             yb=mol[j].y;
             zb=mol[j].z;
 
             r =(xa-xb)*(xa-xb);
             r+=(ya-yb)*(ya-yb);
             r+=(za-zb)*(za-zb);
             r = sqrt(r);

             distM.push_back(r);

             contM.push_back(-1);

         }
     }

     /* set coupling for AB */
     for (int i=0; i<alist0.size(); i++) {
          int ia=alist0[i].id0;
          int ib=alist0[i].id1;

          id=ij2index(ia, ib);

          alist0[i].r=distM[id];
          alist0[i].e=ab[0]*exp(-ab[1]*(alist0[i].r-ab[2]));
          alist0[i].tda=-log10(alist0[i].e);
          alist0[i].cont=0;

          contM[id]=0;

     }

     /* set coupling for C */
     for (int i=0; i<clist0.size(); i++) {
          int ia=clist0[i].id0;
          int ib=clist0[i].id1;

          id=ij2index(ia, ib);

          /* when ab is assigned, ignore this */
          if (contM[id] >= 0) {continue;}

          clist0[i].r=distM[id];
          clist0[i].e=cb[0]*exp(-cb[1]*(clist0[i].r-cb[2]));
          clist0[i].tda=-log10(clist0[i].e);
          clist0[i].cont=0;

          contM[id]=0;

     }

     /* set coupling for H */
     for (int i=0; i<hlist0.size(); i++) {
          int ia=hlist0[i].id0;
          int ib=hlist0[i].id1;

          id=ij2index(ia, ib);

          /* when ab is assigned, ignore this */
          if (contM[id] >= 0) {continue;}

          hlist0[i].r=distM[id];

          hlist0[i].e=hb[0]*exp(-hb[1]*(hlist0[i].r-hb[2]));
          hlist0[i].tda=-log10(hlist0[i].e);

          if (hlist0[i].tda<0.0) {
              hlist0[i].cont=-1;
              continue;
          }

          hlist0[i].cont=1;

          contM[id]=1;
     }

     /* set coupling for S */
     for (int i=0; i<mol.size(); i++) {
         for (int j=0; j<i; j++) {

              id=ij2index(i, j);

              if (distM[id] > radcut_space) {continue;}
              if (contM[id] >= 0) {continue;}

              thisBond.r=distM[id];
              thisBond.e=sb[0]*exp(-sb[1]*(thisBond.r-sb[2]));
              thisBond.tda=-log10(thisBond.e);

              if (thisBond.tda<0.0) {
                  continue;
              }

              thisBond.cont=2;

              /*id starts from 0*/
              thisBond.id0=i;
              thisBond.id1=j;

              //cout <<i <<"  "<< thisBond.tda <<endl;

              slist0.push_back(thisBond);
         }
     }

     distM.erase(distM.begin(), distM.end());
     contM.erase(contM.begin(), contM.end());

     return EXIT_SUCCESS;

}

/*---------------------------------------------------------------------*/
/* Get index for triangle matrix */
inline int ij2index(int i, int j)
/*---------------------------------------------------------------------*/
{
    int a;

    if (i >= j) return ( (i*i+i)/2+j );
    else return ( (j*j+j)/2+i );

}

