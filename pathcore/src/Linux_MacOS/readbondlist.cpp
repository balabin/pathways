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

#include "bondlist.h"

// read bond list file
int readBondList(string &file, BondList &blist) {

     // sanity check
     ifstream sin (file.c_str());
     if (!sin) {
         cout << "ERROR [readBondList]: can not open " << file << endl;
         return EOF;
     };

     // read records
     string s;
     Bond thisBond;

     typedef vector< string > split_vector_type;
     split_vector_type SplitVec;

     // beacuse id begins from 1 in the file, transfer them here.
     while (getline(sin, s)) {
         split( SplitVec, s, is_any_of(" ") );
         thisBond.id0 = atoi(SplitVec[0].c_str())-1;
         thisBond.id1 = atoi(SplitVec[1].c_str())-1;
         if (thisBond.id0 < 0 || thisBond.id1 < 0) {
             cout << "ERROR [readBondList]: wrong id " << thisBond.id0 << " " << thisBond.id1 << endl;
         }
         blist.push_back(thisBond);
     };

     sin.close();
     return EXIT_SUCCESS;

};

