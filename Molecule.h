#ifndef _Molecule_
#define _Molecule_

#include <vector>
#include "Atom.h"
#include "Residue.h"

class Molecule: public std::vector<Residue> {
public:
	void add(Residue res);
	void add(Atom atom);
	void getBonds();
	void getAngles();
	void getDihedral();
	void getNatCont(float dfcontact);
};

#endif
