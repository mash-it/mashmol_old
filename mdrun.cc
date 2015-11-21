#include <iostream>
#include <fstream>

#include "util.h"

using namespace std;

class Stretch
{
public:
	int n1, n2;
	float length; // bond length
};

class Bend
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
	vector<Stretch> stretch;
	vector<Bend> bend;
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
	void setNForces(int s, int a, int d, int c) {
		stretch.resize(s);
		bend.resize(a);
		dihedral.resize(d);
		contact.resize(c);
	}
	void setCoord(int i, float x, float y, float z) {
		rx[i] = x;
		ry[i] = y;
		rz[i] = z;
	}
	void setStretches(int i, int n1, int n2, float length) {
		stretch[i].n1 = n1;
		stretch[i].n2 = n2;
		stretch[i].length = length;
	}
	void setBends(int i, int n1, int n2, int n3, float angle) {
		bend[i].n1 = n1;
		bend[i].n2 = n2;
		bend[i].n3 = n3;
		bend[i].angle = angle;
	}
	void setDiheds(int i, int n1, int n2, int n3, int n4, float d) {
		dihedral[i].n1 = n1;
		dihedral[i].n2 = n2;
		dihedral[i].n3 = n3;
		dihedral[i].n4 = n4;
		dihedral[i].dangle = d;
	}
	void setContacts(int i, int n1, int n2, float distance) {
		contact[i].n1 = n1;
		contact[i].n2 = n2;
		contact[i].distance = distance;
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

		int nstretchs=0, nbends=0, ndiheds=0, nconts=0;
		while (std::getline(forceFile, bufferLine)) {
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "STRETCH") nstretchs++;
			if (record == "BEND") nbends++;
			if (record == "DIHED") ndiheds++;
			if (record == "CONTACT") nconts++;
		}
		md.setNForces(nstretchs, nbends, ndiheds, nconts);
		cout << nstretchs << " " << nbends << " " << ndiheds << " " << nconts << endl;
		forceFile.clear();

		forceFile.seekg(0, ios_base::beg);
		int istretchs=0, ibends=0, idiheds=0, iconts=0;
		int n1, n2, n3, n4;
		float val;

		while (std::getline(forceFile, bufferLine)) {;
			record = util::removeSpaces(bufferLine.substr(0,8));
			if (record == "STRETCH") {
				n1 = stoi(bufferLine.substr( 8,8));
				n2 = stoi(bufferLine.substr(16,8));
				val= stof(bufferLine.substr(24,8));
				md.setStretches(istretchs, n1, n2, val);
				istretchs++;
			}
			if (record == "BEND") {
				n1 = stoi(bufferLine.substr( 8,8));
				n2 = stoi(bufferLine.substr(16,8));
				n3 = stoi(bufferLine.substr(24,8));
				val= stof(bufferLine.substr(32,8));
				md.setBends(ibends, n1, n2, n3, val);
				ibends++;
			}
			if (record == "DIHED") {
				n1 = stoi(bufferLine.substr( 8,8));
				n2 = stoi(bufferLine.substr(16,8));
				n3 = stoi(bufferLine.substr(24,8));
				n3 = stoi(bufferLine.substr(24,8));
				n4 = stoi(bufferLine.substr(32,8));
				val= stof(bufferLine.substr(40,8));
				md.setDiheds(idiheds, n1, n2, n3, n4, val);
				idiheds++;
			}
			if (record == "CONTACT") {
				n1 = stoi(bufferLine.substr( 8,8));
				n2 = stoi(bufferLine.substr(16,8));
				val= stof(bufferLine.substr(24,8));
				md.setContacts(iconts, n1, n2, val);
				iconts++;
			}
		}

		cout << istretchs << " " << ibends << " " << idiheds << " " << iconts << endl;

		forceFile.close();
	}
}
