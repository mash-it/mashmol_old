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
void Molecule::getBonds() {
	for(int i=0; i<size()-1; i++) {
		std::cout << std::setw(7) << "BOND" << ' ';
		std::cout << std::setw(7) << at(i).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+1).getSeq() << ' ';
		std::cout << std::setw(8) << at(i).getCA().distance(at(i+1).getCA()) << '\n';
	}
}
void Molecule::getAngles() {
	for(int i=0; i<size()-2; i++) {
		std::cout << std::setw(7) << "ANGLE" << ' ';
		std::cout << std::setw(7) << at(i).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+1).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+2).getSeq() << ' ';
		std::cout << std::setw(8) << Atom::angle(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA()) << '\n';
	}
}
void Molecule::getDihedral() {
	for(int i=0; i<size()-3; i++) {
		std::cout << std::setw(7) << "DIHED" << ' ';
		std::cout << std::setw(7) << at(i).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+1).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+2).getSeq() << ' ';
		std::cout << std::setw(7) << at(i+3).getSeq() << ' ';
		std::cout << std::setw(8) << Atom::dihedral(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA(), at(i+3).getCA()) << '\n';
	}
}
void Molecule::getNatCont(float dfcontact) {
	for(int i=0; i<size(); i++) { for(int j=0; j<size(); j++) {
		if(at(i).getSeq() >= at(j).getSeq()-3) continue;
		float dist = at(i).distance(at(j));
		if (dist < dfcontact) {
			std::cout << std::setw(7) << "CONT" << ' ';
			std::cout << std::setw(7) << at(i).getSeq() << ' ';
			std::cout << std::setw(7) << at(j).getSeq() << ' ';
			std::cout << std::setw(8) << dist << '\n';
		}
	}}
}
