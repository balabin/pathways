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

// regular expressions
#include "pdb.h"

// remove spaces from string
string despace (string s) {

   string s1;

   s1=s;
   boost::algorithm::replace_all(s1, " ", "");

   return s1;
// return boost::algorithm::replace_all(s, " ", "");

};

// read PDB file
int readPdb(string &file, Pdb &mol) {

     // sanity check
     ifstream sin (file.c_str());
     if (!sin) {
         cout << "ERROR [readPdb]: can not open " << file << endl;
         return EOF;
     };

     // read records
     boost::regex re;
     re.assign("^(ATOM\\s{2}|HETATM).*$");
     string s;
     Atom thisatom;
     while (getline(sin, s)) {
         // only read ATOM and HETATM records
         if (!boost::regex_match(s, re)) continue;
         thisatom.id = atoi(s.substr(6, 5).c_str());
         thisatom.name = despace(s.substr(12, 4));
         thisatom.resname = despace(s.substr(17, 4));
         thisatom.chain = despace(s.substr(21, 1));
         thisatom.resid = atoi(s.substr(22, 3).c_str());
         thisatom.x = atof(s.substr(30, 8).c_str());
         thisatom.y = atof(s.substr(38, 8).c_str());
         thisatom.z = atof(s.substr(46, 8).c_str());
         thisatom.occupancy = atof(s.substr(54, 6).c_str());
         thisatom.beta = atof(s.substr(60, 6).c_str());
         thisatom.segid = despace(s.substr(72, 4));
         mol.push_back(thisatom);
     };

     sin.close();
     return EXIT_SUCCESS;

};

