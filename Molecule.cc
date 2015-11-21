#include <iomanip>
#include <sstream> // stringstream
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

std::string Molecule::writeStretches() {
	std::stringstream ss;
	for(int i=0; i<size()-1; i++) {
		ss << std::setw(7) << "STRETCH" << ' ';
		ss << std::setw(7) << at(i).getSeq() << ' ';
		ss << std::setw(7) << at(i+1).getSeq() << ' ';
		ss << std::setw(8) << at(i).getCA().distance(at(i+1).getCA()) << '\n';
	}
	return ss.str();
}
std::string Molecule::writeAngles() {
	std::stringstream ss;
	for(int i=0; i<size()-2; i++) {
		ss << std::setw(7) << "ANGLE" << ' ';
		ss << std::setw(7) << at(i).getSeq() << ' ';
		ss << std::setw(7) << at(i+1).getSeq() << ' ';
		ss << std::setw(7) << at(i+2).getSeq() << ' ';
		ss << std::setw(8) << Atom::angle(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA()) << '\n';
	}
	return ss.str();
}
std::string Molecule::writeDihedral() {
	std::stringstream ss;
	for(int i=0; i<size()-3; i++) {
		ss << std::setw(7) << "DIHED" << ' ';
		ss << std::setw(7) << at(i).getSeq() << ' ';
		ss << std::setw(7) << at(i+1).getSeq() << ' ';
		ss << std::setw(7) << at(i+2).getSeq() << ' ';
		ss << std::setw(7) << at(i+3).getSeq() << ' ';
		ss << std::setw(8) << Atom::dihedral(at(i).getCA(), at(i+1).getCA(), at(i+2).getCA(), at(i+3).getCA()) << '\n';
	}
	return ss.str();
}
std::string Molecule::writeNatCont(float dfcontact) {
	std::stringstream ss;
	for(int i=0; i<size(); i++) { for(int j=0; j<size(); j++) {
		if(at(i).getSeq() >= at(j).getSeq()-3) continue;
		float dist = at(i).distance(at(j));
		if (dist < dfcontact) {
			ss << std::setw(7) << "CONT" << ' ';
			ss << std::setw(7) << at(i).getSeq() << ' ';
			ss << std::setw(7) << at(j).getSeq() << ' ';
			ss << std::setw(8) << dist << '\n';
		}
	}}
	return ss.str();
}
