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
#ifndef _PDB_H_
#define _PDB_H_

#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>

#include <boost/regex.hpp>
#include <boost/functional.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

// atom attributes in a PDB file
typedef struct Atom {

   int id;
   string name;
   string resname;
   string chain;
   int resid;
   double x;
   double y;
   double z;
   double occupancy;
   double beta;
   string segid;

/*
   // coordinates as vector
   vector<double> xyz() {
     vector<double> r;
     r.push_back(x);
     r.push_back(y);
     r.push_back(z);
     return r;
   };
*/

};

// molecule
typedef vector<Atom> Pdb;

// remove spaces from a string
string despace(string);

// read PDB
int readPdb(string &file, Pdb &mol);

#endif

