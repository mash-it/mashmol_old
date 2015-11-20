#ifndef _Molecule_
#define _Molecule_

#include <vector>
#include "Atom.h"
#include "Residue.h"

class Molecule: public std::vector<Residue> {
public:
	void add(Residue res);
	void add(Atom atom);
	void writeBonds(std::ostream& os);
	void writeAngles(std::ostream& os);
	void writeDihedral(std::ostream& os);
	void writeNatCont(std::ostream& os, float dfcontact);
};

#endif
