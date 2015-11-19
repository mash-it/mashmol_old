#include <iostream>
#include <iomanip> // setw
#include <algorithm> // remove

#include "Atom.h"

namespace util {
	std::string removeSpaces(std::string s) {
		s.erase(remove(s.begin(), s.end(), ' '),s.end());
		return s;
	}
};

Atom::Atom(std::string line): r(0,0,0) {
	serial = stoi(line.substr(6,5));
	name = util::removeSpaces(line.substr(12,4));
	altLoc = line.substr(16,1);
	resName = line.substr(17,3);
	chainID = line.substr(21,1);
	resSeq = stoi(line.substr(22,4));
	iCode = line.substr(26,1);
	occupancy = stof(line.substr(54,6));
	tempFactor = stof(line.substr(60,6));
	element = line.substr(76,2);
	charge = line.substr(78,2);

	float x = stof(line.substr(30,8));
	float y = stof(line.substr(38,8));
	float z = stof(line.substr(46,8));
	r.setCoord(x,y,z);
}

float Atom::distance(Atom target) {
	return (r - target.r).norm();
}

float Atom::angle(Atom left, Atom center, Atom right) {
	return Vector3D::angle(left.r, center.r, right.r);
}

float Atom::dihedral(Atom a, Atom b, Atom c, Atom d) {
	return Vector3D::dihedral(a.r, b.r, c.r, d.r);
}

int Atom::getResSeq() {
	return resSeq;
}

std::string Atom::getName() {
	return name;
}

void Atom::show() {
	std::cout << std::setw(8) << resSeq;
	std::cout << std::setw(8) << r.x;
	std::cout << std::setw(8) << r.y;
	std::cout << std::setw(8) << r.z;
	std::cout << "  # " << std::setw(4) << resSeq << " " << resName;
	std::cout << '\n';
}


