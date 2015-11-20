#ifndef _Residue_
#define _Residue_

#include "Atom.h"
#include <vector>

class Residue: public std::vector<Atom> {
	int seq;
public:
	Residue();
	int getSeq();
	void add(Atom atom);
	float distance(Residue other);
	Atom getCA();
};

#endif
