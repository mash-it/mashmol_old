CO=-std=c++11
PROGS=pdb2ninfo mdrun
PN_CC=Vector3D.cc Atom.cc Residue.cc Molecule.cc pdb2ninfo.cc
PN_H=Vector3D.h Atom.h Residue.h Molecule.h
MD_CC=mdrun.cc

all: $(PROGS)

clean:
	rm $(PROGS)

pdb2ninfo: $(PN_CC) $(PN_H)
	g++ $(PN_CC) $(CO) -o pdb2ninfo

mdrun: $(MD_CC) $(MD_H)
	g++ $(MD_CC) $(CO) -o mdrun

