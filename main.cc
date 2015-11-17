#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cmath> // pow, sqrt
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
	Atom(string line): r(0,0,0)
	{
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
	};
	float distance(Atom target) {
		return (r - target.r).norm();
	}
	void show() {
		cout << r << " " << r.norm() << endl;
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

	cout << inpfile << endl;

	ifstream fin(inpfile.c_str());

	// open pdb file
	if (fin.fail())
	{
		cerr << "failed to open " << inpfile << endl;
		return 1;
	}

	// read pdb file
	string line;
	while ( getline(fin, line) ) {
		if (line.substr(0,4).compare("ATOM") == 0) {
			Atom atom = Atom(line);
			atom.show();
		}
	}
	return 0;
}

