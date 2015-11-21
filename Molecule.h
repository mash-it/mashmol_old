#ifndef _Molecule_
#define _Molecule_

#include <vector>
#include "Atom.h"
#include "Residue.h"

class Molecule: public std::vector<Residue> {
public:
	void add(Residue res);
	void add(Atom atom);
	std::string writeStretches();
	std::string writeAngles();
	std::string writeDihedral();
	std::string writeNatCont(float dfcontact);
};

#endif
