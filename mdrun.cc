#include <iostream>
#include <fstream>

#include "util.h"

using namespace std;

class MdSystem
{
	// position of particles
	vector<float> rx;
	vector<float> ry;
	vector<float> rz;
	vector<float> vx;
	vector<float> vy;
	vector<float> vz;
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
	void setCoord(int i, float x, float y, float z) {
		rx[i] = x;
		ry[i] = y;
		rz[i] = z;
	}
};

int main(int argc, char *argv[]) {
	std::string atomPath = "atom.dat";
	std::string ninfoPath = "ninfo.dat";
	std::string confPath = "conf.dat";

	// open atom file
	std::ifstream atomFile(atomPath.c_str());
	if (atomFile.fail())
	{
		std::cerr << "failed to open " << atomPath << std::endl;
		return 1;
	}

	std::string line;
	std::string record;


	// read number of atoms
	int natoms = 0;
	while (std::getline(atomFile, line)) {
		record = util::removeSpaces(line.substr(0,8));
		if (record == "ATOM") {
			natoms++;
		}
	}
	atomFile.clear();

	MdSystem md;
	md.setNAtoms(natoms);

	// read atom coordinate
	atomFile.seekg(0, ios_base::beg);
	int i=0;
	float x,y,z;
	while (std::getline(atomFile, line)) {;
		record = util::removeSpaces(line.substr(0,8));
		if (record == "ATOM") {
			x = stof(line.substr( 8,8));
			y = stof(line.substr(16,8));
			z = stof(line.substr(24,8));
			md.setCoord(i,x,y,z);
			i++;
		}
	}
	atomFile.close();
}
