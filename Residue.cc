#include <unistd.h>
#include "Residue.h"

Residue::Residue() {
	seq = -1; // this means no atoms in the residue
}

int Residue::getSeq() {
	return seq;
};

void Residue::add(Atom atom) {
	push_back(atom);
	if (seq == -1) {
		seq = atom.getResSeq();
		return;
	} 
	if (seq != atom.getResSeq()) {
		throw; // invalid sequence of residue
	}
}

float Residue::distance(Residue other) {
	if ((this->size() == 0) || (other.size() == 0)) {
		throw; // empty residue has no distance
	}
	float d = this->at(0).distance(other.at(0));

	for (int i=0; i<this->size(); i++) {
		for (int j=0; j<other.size(); j++) {
			d = std::min(d, this->at(i).distance(other.at(j)));
		}
	}
	return d;
}

Atom Residue::getCA() {
	for (int i=0; i<size(); i++) {
		if(at(i).getName() == "CA") return at(i);
	}
	return at(0);
	//throw "No CA Atom in this residue"; // No CA ATOM in this residue
}
