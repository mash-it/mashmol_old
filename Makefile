CO=-std=c++11
CODE=Vector3D.cc Atom.cc Residue.cc Molecule.cc main.cc
HEAD=Vector3D.h Atom.h Residue.h Molecule.h
mashmol: $(CODE) $(HEAD)
	g++ $(CODE) $(CO) -o mashmol
