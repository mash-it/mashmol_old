CO=-std=c++11
PROGS=pdb2force mdrun
PN_CC=Vector3D.cc Atom.cc Residue.cc Molecule.cc pdb2force.cc
PN_H=Vector3D.h Atom.h Residue.h Molecule.h

MD_H=MdSystem.h
MD_CC=MdSystem.cc mdrun.cc

all: $(PROGS)

clean:
	rm $(PROGS)

pdb2force: $(PN_CC) $(PN_H)
	g++ $(PN_CC) $(CO) -o pdb2force

mdrun: $(MD_CC) $(MD_H)
	g++ $(MD_CC) $(CO) -o mdrun

