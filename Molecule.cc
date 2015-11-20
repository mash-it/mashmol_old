#include <iomanip>
#include "Molecule.h"

void Molecule::add(Residue res) {
	push_back(res);
}
void Molecule::add(Atom atom) {
	for(int i=0; i<size(); i++) {
		if (at(i).getSeq() == atom.getResSeq()) {
			at(i).add(atom);
			return;
		}
	}
	// if no residue for this atom
	add(Residue());
	back().add(atom);
}
void Molecule::writeBonds(std::ostream& os) {
	for(int i=0; i<size()-1; i++) {
		os << std::setw(7) << "BOND" << ' ';
		os << std::setw(7) << at(i).getSeq() << ' ';
		os << std::setw(7) << at(i+1).getSeq() << ' ';
		os << std::setw(8) << at(i).getCA().distance(at(i+1).getCA()) << '\n';
	}
}
void Molecule::writeAngles(std::ostream& os) {
	for(int i=0; i<size()-2; i++) {
		os << std::setw(7) << "ANGLE" << ' ';
		os << std::setw(7) << at(i).getSeq() << ' ';
		os << std::setw(7) << at(i+1).getSeq() << ' ';
		os << std::setw(7) << at(i+2).getSeq() << ' ';
		os << std::setw(8) << Atom::angle(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA()) << '\n';
	}
}
void Molecule::writeDihedral(std::ostream& os) {
	for(int i=0; i<size()-3; i++) {
		os << std::setw(7) << "DIHED" << ' ';
		os << std::setw(7) << at(i).getSeq() << ' ';
		os << std::setw(7) << at(i+1).getSeq() << ' ';
		os << std::setw(7) << at(i+2).getSeq() << ' ';
		os << std::setw(7) << at(i+3).getSeq() << ' ';
		os << std::setw(8) << Atom::dihedral(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA(), at(i+3).getCA()) << '\n';
	}
}
void Molecule::writeNatCont(std::ostream& os, float dfcontact) {
	for(int i=0; i<size(); i++) { for(int j=0; j<size(); j++) {
		if(at(i).getSeq() >= at(j).getSeq()-3) continue;
		float dist = at(i).distance(at(j));
		if (dist < dfcontact) {
			os << std::setw(7) << "CONT" << ' ';
			os << std::setw(7) << at(i).getSeq() << ' ';
			os << std::setw(7) << at(j).getSeq() << ' ';
			os << std::setw(8) << dist << '\n';
		}
	}}
}
