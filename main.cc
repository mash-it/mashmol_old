#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cmath> // pow, sqrt
#include <vector>
#include <map>
#include <string>
using namespace std;

class Vector3D
{
	float x, y, z;
public:
	void setCoord(float argx, float argy, float argz) {
 		x = argx;
		y = argy;
		z = argz;
	}

	Vector3D(float argx, float argy, float argz) {
		setCoord(argx, argy, argz);
	}

	Vector3D operator + (Vector3D other) {
		return Vector3D(x + other.x, y + other.y, z + other.z);
	}

	Vector3D operator - (Vector3D other) {
		return Vector3D(x - other.x, y - other.y, z - other.z);
	}

	friend ostream& operator << (ostream& os, const Vector3D& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	float norm() {
		return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	}

	void show() {
		cout << "(" << x << ", " << y << ", " << z << ")" << endl;
	}
};

class Atom
{
	int serial;
	string name;
	string altLoc;
	string resName;
	string chainID;
	int resSeq;
	string iCode;
	float occupancy;
	float tempFactor;
	string element;
	string charge;
	Vector3D r;

public:
	Atom(string line): r(0,0,0) {
		serial = stoi(line.substr(6,5));
		name = line.substr(12,4);
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
	float distance(Atom target) {
		return (r - target.r).norm();
	}
	int getResSeq() {
		return resSeq;
	}
	void show() {
		cout << serial << " " << resSeq << " " << r << " " << r.norm() << endl;
	}
};

class Residue
{
public:
	// atoms in Residue is vector of pointer, not vector of atom
	vector<Atom> atoms;
	Residue() {}
	void add(Atom p) {
		atoms.push_back(p);
	}
	float distance(Residue t) {
		if ((atoms.size() == 0) || (t.atoms.size() == 0)) {
			throw; // empty residue has no distance
		}
		float d = atoms.at(0).distance(t.atoms.at(0));

		for (int i=0; i<atoms.size(); i++) {
			for (int j=0; j<t.atoms.size(); j++) {
				d = min(d, atoms.at(i).distance(t.atoms.at(j)));
			}
		}
		return d;
	}
};

int main(int argc, char *argv[]) {
	string inpfile = "input.pdb";
	int opt;
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch(opt) {
			case 'f':
				inpfile = optarg;
		}
	}

	ifstream fin(inpfile.c_str());

	// open pdb file
	if (fin.fail())
	{
		cerr << "failed to open " << inpfile << endl;
		return 1;
	}

	// read pdb file
	string line;
	map<int, Residue> residues;

	while ( getline(fin, line) ) {
		if (line.substr(0,4).compare("ATOM") == 0) {
			Atom atom(line);
			residues[atom.getResSeq()].add(atom);
		}
	}

	for (int i=1; i<=76; i++) {
		for (int j=1; j<i-3; j++) {
			float d = residues[i].distance(residues[j]);
			if (d < 6.5) cout << i << " " << j << " " << d << endl;
		}
	}

	return 0;
}

