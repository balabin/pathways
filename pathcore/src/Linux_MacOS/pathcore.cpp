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

#include <math.h>

#include "bondlist.h"

int main (int argc, char *argv[])
{
  Pdb mol;
  BondList clist0;   /* the whole covalent bond list */
  BondList hlist0;   /* the whole H bond list */
  BondList slist0;   /* the whole space list */
  BondList alist0;   /* the artifical list for donor/acceptor */

  string pdbFile;
  string cbFile;
  string hbFile;

  vector<int> nDonor;
  vector<int> nAcceptor;
  vector<int> nBridge;

  /* parameters */
  double cb[3];
  double hb[3];
  double sb[3];
  double ab[3];

  int nD=0, nA=0, nB=0, iActiveAB=0;

  int nPath=0;

  int nSource;
  int nTarget;
  double center[3];
  /* cut off used for through-space coupling */
  double radcut_space=DIST_CUTOFF;
  /* cut off used for generating graph */
  double radcut, RadCutInp=7.0;

  int nVertex;

  CConfigCenter::IntPair_Double_Map m_DirectedEdges;

  /* default paramters from science paper*/
  /*
  cb[0]=0.60;
  cb[1]=0.00;
  cb[2]=0.00;

  hb[0]=0.36;
  hb[1]=1.70;
  hb[2]=2.80;

  sb[0]=0.60;
  sb[1]=1.70;
  sb[2]=1.40;
  */

  /* all default values are gone. */
  /* you must set all the values now */
  cb[0]=-0.60;
  cb[1]=0.00;
  cb[2]=0.00;

  hb[0]=-0.36;
  hb[1]=-1.70;
  hb[2]=-2.80;

  sb[0]=-0.60;
  sb[1]=-1.70;
  sb[2]=-1.40;

  /* artifcal bonds for donor/acceptor atoms */
  ab[0]=1.0;
  ab[1]=0.0;
  ab[2]=0.0;

  /*==========================================================*/
  int nop=0;
  int idonor=0;
  int iaccep=0;
  int ibridg=0;

  /* initialize flags */
  string program = argv[0];

//  from pathways.tcl
//    lappend comlist "-i" "${out}.pdb"
//    lappend comlist "-c" "${out}.cb"
//    lappend comlist "-h" "${out}.hb"
//    lappend comlist "-p" "$p"
//    lappend comlist "-ab" "$cda"
//    lappend comlist "-tscut"  "$tscut"
//    lappend comlist "-procut" "$procut"
//    lappend comlist "-epsc"   "$epsc"
//    lappend comlist "-epsh"   "$epsh"
//    lappend comlist "-exph"   "$exph"
//    lappend comlist "-r0h"    "$r0h"
//    lappend comlist "-epsts"   "$epsts"
//    lappend comlist "-expts"  "$expts"
//    lappend comlist "-r0ts"   "$r0ts"
//    lappend comlist "-q"

  // build the flag-value map
  map<string, string> cmdline;
  for (int i=1; i<argc; i+=2) {
      string thiskey = argv[i];
      if ((i+1)<argc) {
         string thisval = argv[i+1];
         cmdline[thiskey] = thisval;
      }
  };

  // assign values
  /* pdb File name */
  if (cmdline["-i"].empty()) {
      cerr << "Need input pdb file "<<endl;
      nop=0;
      //exit(1);
  }
  pdbFile=cmdline["-i"];
  nop++;

  /* covalend bond list */
  if (cmdline["-c"].empty()) {
      cerr << "Need covalend bond list file "<<endl;
      nop=0;
      //exit(1);
  }
  cbFile=cmdline["-c"];
  nop++;

  /* hydrogen bond list */
  if (cmdline["-h"].empty()) {
      cerr << "Need hydrogen bond list file "<<endl;
      nop=0;
      //exit(1);
  }
  hbFile=cmdline["-h"];
  nop++;

  /* Number of path to be searched for each atom pair */
  if (!cmdline["-p"].empty()) {
      nPath=atoi(cmdline["-p"].c_str());
  }

  if (nPath <= 0) {
      cerr << "nPath must be greater than 0 "<<endl;
      //exit(1);
  }

  if (nop != 3) {
      cerr << "Usage (need four input parameters at least) :" <<endl;
      cerr << "-i      input pdb file "<<endl;
      cerr << "-c      covalent bond list file "<<endl;
      cerr << "-h      hydrogen bond list file "<<endl;
      cerr << "-p      <value> (optional) how many pathways will be searched (default: 1) "<<endl;
      cerr << "-ab             (optional) artifical bond list for D/A "<<endl;
      cerr << "-tscut  <value> (optional) TS cutoff distance (default 6A)" <<endl;
      cerr << "-procut <value> (optional) margin distance for radius cut off in protein (default: 7A)" <<endl;
      cerr << "-epsc   <value> (optional) decay per covalent bond (default 0.6) "<<endl;
      cerr << "-epsh   <value> (optional) decay per hydrogen bond (default 0.36) "<<endl;
      cerr << "-exph   <value> (optional) exponential factor per hydrogen bond (default 1.7A) "<<endl;
      cerr << "-r0h    <value> (optional) distance offset per hydrogen bond (default 2.8A) "<<endl;
      cerr << "-epsts  <value> (optional) decay prefactor TS jump (default 0.6) "<<endl;
      cerr << "-expts  <value> (optional) exponential factor per TS jump (default 1.7A) "<<endl;
      cerr << "-r0ts   <value> (optional) distance offset per TS jump (default 1.4A) "<<endl;
      cerr << "-q      (optional) calculate importance of bridge atoms" <<endl;
      exit(1);
  }

  /* artifical bonds */
  iActiveAB=0;
  if (!cmdline["-ab"].empty()) {
      ab[0]=atof(cmdline["-ab"].c_str());
      iActiveAB=1;
  }

  /* Through-Space (TS) cutoff distance (default 6A) */ 
  if (!cmdline["-tscut"].empty()) {
      radcut_space=atof(cmdline["-tscut"].c_str()); 
  }

  /* Protein cutoff distance (default 7A) to calculate distance matrix */ 
  if (!cmdline["-procut"].empty()) {
      RadCutInp=atof(cmdline["-procut"].c_str());
  }

  /* decay per covalent bond (default 0.6) */
  if (!cmdline["-epsc"].empty()) {
      cb[0]=atof(cmdline["-epsc"].c_str());
  }

  /* decay prefactor per hydrogen bond (default 0.36) */
  if (!cmdline["-epsh"].empty()) {
      hb[0]=atof(cmdline["-epsh"].c_str());
  }

  /* exponential factor per hydrogen bond (default 1.7A) */
  if (!cmdline["-exph"].empty()) {
      hb[1]=atof(cmdline["-exph"].c_str());
  }

  /* distance offset per hydrogen bond (default 2.8A) */
  if (!cmdline["-r0h"].empty()) {
      hb[2]=atof(cmdline["-r0h"].c_str());
  }

  /* -epsts <value> - decay prefactor TS jump (default 0.6) */
  if (!cmdline["-epsts"].empty()) {
      sb[0]=atof(cmdline["-epsts"].c_str());
  }
  /* exponential factor per TS jump (default 1.7A) */
  if (!cmdline["-expts"].empty()) {
      sb[1]=atof(cmdline["-expts"].c_str());
  }

  /* distance offset per TS jump (default 1.4A) */
  if (!cmdline["-r0ts"].empty()) {
      sb[2]=atof(cmdline["-r0ts"].c_str());
  }

  /* read extra input options (can not be seen by normal users) */
  /* must be put behind the above options */

  nB=0;
  for (int i=1; i<argc; i++) {
      if (argv[i][0] == '-') {
            if (argv[i][1] == 'd') {
              /* "-d" (optional) */
              /*
              for (int j=i+1; j<argc; j++) {
                   if (argv[j][0] == '-') { 
                       break;
                   }
                   nDonor.push_back(atoi(argv[j]));
              }
              nD=nDonor.size();
              idonor=1;
              */
            } else if (argv[i][1] == 'a' && argv[i][2] != 'b') {
              /* "-a" (optional) */
              /*
              for (int j=i+1; j<argc; j++) {
                   if (argv[j][0] == '-') { 
                       break;
                   }
                   nAcceptor.push_back(atoi(argv[j]));
              }
              nA=nAcceptor.size();
              iaccep=1;
              */
            } else if (argv[i][1] == 'q') {
              /* "-q" : optional */
              for (int j=i+1; j<argc; j++) {
                   if (argv[j][0] == '-') { 
                       break;
                   }
                   nBridge.push_back(atoi(argv[j]));
              }
              nB=nBridge.size();
              ibridg=1;
            } else if (argv[i][1] == 'C') {
              /* "-C" 3 parameters for covalent bond */
              cb[0] =atof(argv[i+1]);
              cb[1] =atof(argv[i+2]);
              cb[2] =atof(argv[i+3]);
            } else if (argv[i][1] == 'H') {
              /* "-H" 3 parameters for hydrogen bond */
              hb[0] =atof(argv[i+1]);
              hb[1] =atof(argv[i+2]);
              hb[2] =atof(argv[i+3]);
            } else if (argv[i][1] == 'S') {
              /* "-S" 3 parameters for space */
              sb[0] =atof(argv[i+1]);
              sb[1] =atof(argv[i+2]);
              sb[2] =atof(argv[i+3]);
            }
       }
   }

  if (RadCutInp < 7.0) {
      cerr << "The margin distance should be bigger than 7 angstrom! "<< RadCutInp <<endl;
      exit(1);
  }

  /* check the values for cb, hb, and sb */
  for (int i=0; i<3; i++) {
    if (cb[i] < 0.0 ) {
        cerr << " Wrong parameters for covalent bond! "<< cb[i] <<endl;
        exit(1);
    }
    if (hb[i] < 0.0 ) {
        cerr << " Wrong parameters for hydrogen bond! "<< hb[i] <<endl;
        exit(1);
    }
    if (sb[i] < 0.0 ) {
        cerr << " Wrong parameters for through space! "<< sb[i] <<endl;
        exit(1);
    }
  }

  /*==========================================================*/
  /* read pdb */
  readPdb(pdbFile, mol);
  //cout << "PDB file is "<< pdbFile.c_str() <<endl;
  //cout << "Total number of atoms is "<< mol.size() <<endl;
  nVertex=mol.size();

  /*==========================================================*/
  /* Check donor, acceptor, or bridge atoms are input or not. 
     If not, set those atoms using the occupancy in pdb */

  /* read donor from pdb: id starts from 1 */
  if (idonor == 0 || nD == 0) {
      nD=0;
      for (int i=0; i<mol.size(); i++) {
          if (mol[i].occupancy == -1.0) {
              nDonor.push_back(i+1);
              nD++;
          }
      }
      if (nD <= 0) {
          cout << "Can not find the donor atom ids." <<endl;
          exit(1);
      }
  }

  /* read acceptor from pdb: id starts from 1 */
  if (iaccep == 0 || nA == 0) {
      nA=0;
      for (int i=0; i<mol.size(); i++) {
          if (mol[i].occupancy == 1.0) {
              nAcceptor.push_back(i+1);
              nA++;
          }
      }
      if (nA <= 0) {
          cout << "Can not find the acceptor atom ids." <<endl;
          exit(1);
      }
  }

  /* read bridge from pdb: id starts from 1 */
  if (ibridg == 1 && nB == 0) {
      nB=0;
      for (int i=0; i<mol.size(); i++) {
          if (mol[i].occupancy == 0.0) {
              nBridge.push_back(i+1);
              nB++;
          }
      }
      if (nB <= 0) {
          cout << "Can not find the bridge atom ids." <<endl;
          exit(1);
      }
  }

  /* build artifical bonds for donor/acceptor */
  if ((nA > 1 || nD >1) && iActiveAB > 0) {
      Bond thisBond; 
      for (int i=0; i<nA; i++) {
          for (int j=0; j<i; j++) {
              thisBond.id0=nAcceptor[i]-1; /* id starts from zero */
              thisBond.id1=nAcceptor[j]-1;
              alist0.push_back(thisBond);
          }
      }
      for (int i=0; i<nD; i++) {
          for (int j=0; j<i; j++) {
              thisBond.id0=nDonor[i]-1; /* id starts from zero */
              thisBond.id1=nDonor[j]-1;
              alist0.push_back(thisBond);
          }
      }
  }

  /* read bond list */
  readBondList(cbFile, clist0);

  /* read H-bond list */
  readBondList(hbFile, hlist0);

  /*
  for (int i =0; i<10; i++) {
       cout << blist[i].id0<<"  "<<blist[i].id1 <<endl;
  }
  */

  /* build artifical bonds for same occupancy*/
  /* only build these bonds for the atoms linked by covalent or hydrogen bond */
  Bond thisBond; 
  for (int i=0; i<mol.size(); i++) {
      if (mol[i].occupancy == 1.0 || mol[i].occupancy == -1.0 || mol[i].occupancy == 0.0) continue;
      for (int j=0; j<i; j++) {
          if (mol[j].occupancy == 1.0 || mol[j].occupancy == -1.0 || mol[j].occupancy == 0.0) continue;

          if (mol[j].occupancy == mol[i].occupancy) {
              /* check covalent bonds */
              int itfrag;
              itfrag=0;
              for (int k=0; k<clist0.size(); k++) {
                  int ia=clist0[k].id0;
                  int ib=clist0[k].id1;
                  
                  if ((ia == i && ib == j) || (ia == j && ib == i)) {
                      thisBond.id0=i; /* id starts from zero */
                      thisBond.id1=j;
                      alist0.push_back(thisBond);
                      itfrag=1;
                      break;
                  }
              }
              if (itfrag == 0) {
                  for (int k=0; k<hlist0.size(); k++) {
                      int ia=hlist0[k].id0;
                      int ib=hlist0[k].id1;
                  
                      if ((ia == i && ib == j) || (ia == j && ib == i)) {
                          thisBond.id0=i; /* id starts from zero */
                          thisBond.id1=j;
                          alist0.push_back(thisBond);
                          itfrag=1;
                          break;
                      }
                  }
              }
          }
      }
  }

  /*==========================================================*/
  /* calculate coupling matrix */
  if (CalCoupling(mol, clist0, hlist0, slist0, alist0, radcut_space,
                  cb, hb, sb, ab) < 0) {
      cout << "Can not calculate coupling matrix." <<endl;
      exit(1);
  }

  /*==========================================================*/

  if (nB > 0) goto LWEIGHT; /* calculate importance of bridge atoms */

  /*==========================================================*/
  /* K-Shortest Path Searching */

  //cout << "Path="<< nPath << " " << nA << " " << nD << " " << iActiveAB <<endl;

  if ((nA > 1 || nD >1) && iActiveAB > 0) goto LABCASE;

  if (nPath <= 0) nPath=1;

  for (int i=0; i<nD; i++) {
      nSource=nDonor[i]-1;
      for (int j=0; j<nA; j++) {
          nTarget=nAcceptor[j]-1;

          /* Generate through-space list  and Calculate coupling*/
          /* we need cut the protein to save time for each source and target */

          center[0]=0.5*(mol[nSource].x+mol[nTarget].x);
          center[1]=0.5*(mol[nSource].y+mol[nTarget].y);
          center[2]=0.5*(mol[nSource].z+mol[nTarget].z);

          radcut =(center[0]-mol[nTarget].x)*(center[0]-mol[nTarget].x);
          radcut+=(center[1]-mol[nTarget].y)*(center[1]-mol[nTarget].y);
          radcut+=(center[2]-mol[nTarget].z)*(center[2]-mol[nTarget].z);
          radcut =sqrt(radcut)+RadCutInp;

          if (GenGraph(&m_DirectedEdges, mol, clist0, hlist0, slist0, alist0, center, radcut) < 0) {continue;}

          //cout << "Path From "<< nSource+1 << " to " << nTarget+1 <<" (id starts from 1)"<<" npath="<<nPath<<endl;
          ShortestPathSearching(&m_DirectedEdges, nVertex, nSource, nTarget, nPath);

          m_DirectedEdges.erase(m_DirectedEdges.begin(), m_DirectedEdges.end());

      }
  }

  return 0;

LABCASE:

  int i, j;

  if (nPath <= 0) nPath=nA+nD;

  srand(getpid());

  i=int (rand()/RAND_MAX)*nD;
  j=int (rand()/RAND_MAX)*nA;

  nSource=nDonor[i]-1;
  nTarget=nAcceptor[j]-1;

  if (i >= nD || j >= nA || i < 0 || j < 0) {
      cout << "WRONG IN LABCASE i="<< i << " j= " << j << " nSource= " << nSource << " nTarget= " << nTarget << " nA= " << nA << " " << nD << " " <<endl;
      exit(1);
  }

  /* Generate through-space list  and Calculate coupling*/
  /* we need cut the protein to save time for each source and target */

  center[0]=0.5*(mol[nSource].x+mol[nTarget].x);
  center[1]=0.5*(mol[nSource].y+mol[nTarget].y);
  center[2]=0.5*(mol[nSource].z+mol[nTarget].z);

  radcut =(center[0]-mol[nTarget].x)*(center[0]-mol[nTarget].x);
  radcut+=(center[1]-mol[nTarget].y)*(center[1]-mol[nTarget].y);
  radcut+=(center[2]-mol[nTarget].z)*(center[2]-mol[nTarget].z);
  radcut =sqrt(radcut)+RadCutInp;

  GenGraph(&m_DirectedEdges, mol, clist0, hlist0, slist0, alist0, center, radcut);

  //cout << "Path From "<< nSource+1 << " to " << nTarget+1 <<" (id starts from 1)"<<endl;
  ShortestPathSearching(&m_DirectedEdges, nVertex, nSource, nTarget, nPath);

  m_DirectedEdges.erase(m_DirectedEdges.begin(), m_DirectedEdges.end());

  return 0;

LWEIGHT:

  if (nPath <= 0) nPath=1;

  /*==========================================================*/
  /* calculate importance of bridge atoms */
  for (int ib=0; ib<nB; ib++) {
      nSource=nBridge[ib]-1;

      /* bridge -> donor */
      /*
      cout << "------------------------------------------------------" <<endl;
      cout << "Path From Bridge atom "<< nSource+1 << " to donor atoms (id starts from 1):" <<endl;
      cout << "------------------------------------------------------" <<endl;
      */
      for (int i=0; i<nD; i++) {
          nTarget=nDonor[i]-1;

          center[0]=0.5*(mol[nSource].x+mol[nTarget].x);
          center[1]=0.5*(mol[nSource].y+mol[nTarget].y);
          center[2]=0.5*(mol[nSource].z+mol[nTarget].z);

          radcut =(center[0]-mol[nTarget].x)*(center[0]-mol[nTarget].x);
          radcut+=(center[1]-mol[nTarget].y)*(center[1]-mol[nTarget].y);
          radcut+=(center[2]-mol[nTarget].z)*(center[2]-mol[nTarget].z);
          radcut =sqrt(radcut)+RadCutInp;

          if (GenGraph(&m_DirectedEdges, mol, clist0, hlist0, slist0, alist0, center, radcut) < 0) {continue;}

          //cout << endl << "*Donor atom id: " << nTarget+1 <<" (id starts from 1)"<<endl;
          ShortestPathSearching(&m_DirectedEdges, nVertex, nSource, nTarget, nPath);

          m_DirectedEdges.erase(m_DirectedEdges.begin(), m_DirectedEdges.end());

      }
      /* bridge -> acceptor */
      /*
      cout << "------------------------------------------------------" <<endl;
      cout << "Path From Bridge atom "<< nSource+1 << " to acceptor atoms:" <<endl;
      cout << "------------------------------------------------------" <<endl;
      */
      for (int i=0; i<nA; i++) {
          nTarget=nAcceptor[i]-1;

          center[0]=0.5*(mol[nSource].x+mol[nTarget].x);
          center[1]=0.5*(mol[nSource].y+mol[nTarget].y);
          center[2]=0.5*(mol[nSource].z+mol[nTarget].z);

          radcut =(center[0]-mol[nTarget].x)*(center[0]-mol[nTarget].x);
          radcut+=(center[1]-mol[nTarget].y)*(center[1]-mol[nTarget].y);
          radcut+=(center[2]-mol[nTarget].z)*(center[2]-mol[nTarget].z);
          radcut =sqrt(radcut)+RadCutInp;

          if (GenGraph(&m_DirectedEdges, mol, clist0, hlist0, slist0, alist0, center, radcut) < 0) {continue;}

          //cout << endl << "*Acceptor atom id: " << nTarget+1 <<" (id starts from 1)"<<ib<<endl;
          ShortestPathSearching(&m_DirectedEdges, nVertex, nSource, nTarget, nPath);

          m_DirectedEdges.erase(m_DirectedEdges.begin(), m_DirectedEdges.end());

      }
  }

  return 0;
}
