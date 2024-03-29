#ifndef _Atom_
#define _Atom_

#include <string>
#include "Vector3D.h"

class Atom
{
	int serial;
	std::string name;
	std::string altLoc;
	std::string resName;
	std::string chainID;
	int resSeq;
	std::string iCode;
	float occupancy;
	float tempFactor;
	std::string element;
	std::string charge;
	Vector3D r;

public:
	int mdIndex;	// for MD index, not index in PDB.

	Atom(std::string line);
	float distance(Atom target);
	static float angle(Atom left, Atom center, Atom right);
	static float dihedral(Atom a, Atom b, Atom c, Atom d);
	int getResSeq();
	float getCoord(int xyz);
	std::string getName();
	std::string getResName();
	std::string write();
};

#endif
