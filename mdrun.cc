#include <iostream>
#include <fstream>

#include "util.h"

using namespace std;

class Bond
{
public:
	int n1, n2;
	float length; // bond length
};

class Angle
{
public:
	int n1, n2, n3;
	float angle; // bond angle
};

class Dihedral
{
public:
	int n1, n2, n3, n4;
	float dangle; // dihedral angle
};

class Contact
{
public:
	int n1, n2;
	float distance;
};

class MdSystem
{
	// position of particles
	vector<float> rx;
	vector<float> ry;
	vector<float> rz;
	vector<float> vx;
	vector<float> vy;
	vector<float> vz;
	vector<Bond> bond;
	vector<Angle> angle;
	vector<Dihedral> dihedral;
	vector<Contact> contact;

public:
	MdSystem() {}
	void setNAtoms(int natoms) {
		rx.resize(natoms);
		ry.resize(natoms);
		rz.resize(natoms);
		vx.resize(natoms);
		vy.resize(natoms);
		vz.resize(natoms);
	}
	void setNForces(int b, int a, int d, int c) {
		bond.resize(b);
		angle.resize(a);
		dihedral.resize(d);
		contact.resize(c);
	}
	void setCoord(int i, float x, float y, float z) {
		rx[i] = x;
		ry[i] = y;
		rz[i] = z;
	}
};

int main(int argc, char *argv[]) {
	std::string atomPath = "atom.dat";
	std::string forcePath = "force.dat";
	std::string confPath = "conf.dat";
	std::string bufferLine, record;
	MdSystem md;

	// read atom file
	{
		std::ifstream atomFile(atomPath.c_str());
		if (atomFile.fail())
		{
			std::cerr << "failed to open " << atomPath << std::endl;
			return 1;
		}
	
		// read number of atoms
		int natoms = 0;
		while (std::getline(atomFile, bufferLine)) {
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "ATOM") {
				natoms++;
			}
		}
		atomFile.clear();
	
		md.setNAtoms(natoms);
	
		// read atom coordinate
		atomFile.seekg(0, ios_base::beg);
		int i=0;
		float x,y,z;
		while (std::getline(atomFile, bufferLine)) {;
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "ATOM") {
				x = stof(bufferLine.substr( 8,8));
				y = stof(bufferLine.substr(16,8));
				z = stof(bufferLine.substr(24,8));
				md.setCoord(i,x,y,z);
				i++;
			}
		}
		atomFile.close();
	}

	// read force file
	{
		std::ifstream forceFile(forcePath.c_str());

		int nbonds=0, nangles=0, ndiheds=0, nconts=0;
		while (std::getline(forceFile, bufferLine)) {
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "BOND") nbonds++;
			if (record == "ANGLE") nangles++;
			if (record == "DIHED") ndiheds++;
			if (record == "CONT") nconts++;
		}
		md.setNForces(nbonds, nangles, ndiheds, nconts);
		cout << nbonds << " " << nangles << " " << ndiheds << " " << nconts << endl;
		forceFile.clear();

		forceFile.seekg(0, ios_base::beg);
		int i=0;
		float x,y,z;
		while (std::getline(forceFile, bufferLine)) {;
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "BOND") {
				
			}
		}
		forceFile.close();
	}
}
