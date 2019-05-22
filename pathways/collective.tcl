#######################################################################
#
# Collective electronic states for the Pathways plugin v. 1.1 and above.
#
# Segid "ANY" matches all segments. Resid "0" matches all residues.
# Change either/both to specific values to customize collective states.
# Use one segid - resid pair per line.
#
# (C) Ilya A. Balabin, Xiangqian Hu, and David N. Beratan 2012
#
#######################################################################

#segid	resid	resname	atom1, atom2, atom3,...

# Nucleic acid bases
ANY	0	ADE	N9 C5 N7 C8 N1 C2 N3 C4 C6 N6
ANY	0	GUA	N9 C4 N2 N3 C2 N1 C6 O6 C5 N7 C8
ANY	0	THY	N1 C6 C2 O2 N3 C4 O4 C5
ANY	0	CYT	N1 C6 C5 C2 O2 N3 C4 N4

# Protein aromatic side chains
ANY	0	PHE	CG CD1 CE1 CZ  CE2 CD2
ANY	0	TRP	CG CD1 NE1 CE2 CD2 CE3 CZ3 CH2 CZ2
ANY	0	TYR	CG CD1 CE1 CZ  CE2 CD2 OH
ANY	0	HIS	CG ND1 CE1 NE2 CD2
ANY	0	HSD	CG ND1 CE1 NE2 CD2
ANY	0	HSP	CG ND1 CE1 NE2 CD2
