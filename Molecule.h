#ifndef _Molecule_
#define _Molecule_

#include <vector>
#include "Atom.h"
#include "Residue.h"

class Molecule: public std::vector<Residue> {
public:
	void add(Residue res);
	void add(Atom atom);
	std::string writeAtoms();
	std::string writeStretches();
	std::string writeBends();
	std::string writeDihedral();
	std::string writeNatCont(float dfcontact);
};

#endif
