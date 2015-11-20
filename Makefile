CO=-std=c++11
CODE=Vector3D.cc Atom.cc Residue.cc Molecule.cc main.cc
HEAD=Vector3D.h Atom.h Residue.h Molecule.h
pdb2ninfo: $(CODE) $(HEAD)
	g++ $(CODE) $(CO) -o pdb2ninfo
