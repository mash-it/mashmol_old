#include <iostream>
#include <iomanip> // setw, setprecision
#include <fstream> // ifstream
#include <unistd.h>
#include <cmath> // pow, sqrt
#include <algorithm> // remove
#include <vector>
using namespace std;

const float pi = 3.14159265;

namespace util {
	string removeSpaces(string s) {
		s.erase(remove(s.begin(), s.end(), ' '),s.end());
		return s;
	}
};

class Vector3D
{
public:
	float x, y, z;
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
	float operator * (Vector3D other) {
		return this->x * other.x + this->y * other.y + this->z * other.z;
	}
	float norm() {
		return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
	}
	float angle(Vector3D left, Vector3D right) {
		Vector3D a = left  - *this;
		Vector3D b = right - *this;
		return acos((a*b) / (a.norm()*b.norm())) * (180/pi);
	}

	friend ostream& operator << (ostream& os, const Vector3D& v) {
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
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
	float distance(Atom target) {
		return (r - target.r).norm();
	}
	float angle(Atom left, Atom right) {
		return r.angle(left.r, right.r);
	}
	int getResSeq() {
		return resSeq;
	}
	string getName() {
		return name;
	}
	void show() {
		cout << setw(8) << resSeq;
		cout << setw(8) << r.x;
		cout << setw(8) << r.y;
		cout << setw(8) << r.z;
		cout << "  # " << setw(4) << resSeq << " " << resName;
		cout << '\n';
	}
}; // Atom

class Residue: public vector<Atom> {
	int seq;
public:
	int getSeq() {
		return seq;
	};
	Residue() {
		seq = -1; // this means no atoms in the residue
	}
	void add(Atom atom) {
		push_back(atom);
		if (seq == -1) {
			seq = atom.getResSeq();
			return;
		} 
		if (seq != atom.getResSeq()) {
			throw; // invalid sequence of residue
		}
	}
	float distance(Residue other) {
		if ((this->size() == 0) || (other.size() == 0)) {
			throw; // empty residue has no distance
		}
		float d = this->at(0).distance(other.at(0));

		for (int i=0; i<this->size(); i++) {
			for (int j=0; j<other.size(); j++) {
				d = min(d, this->at(i).distance(other.at(j)));
			}
		}
		return d;
	}
	Atom getCA() {
		for (int i=0; i<size(); i++) {
			if(at(i).getName() == "CA") return at(i);
		}
		return at(0);
		//throw "No CA Atom in this residue"; // No CA ATOM in this residue
	}
}; // Residue

class NativeContact {
	Residue res1, res2;	
	float natdist;
public:
	NativeContact(Residue residue1, Residue residue2, float distance) {
		res1 = residue1;
		res2 = residue2;
		natdist = distance;
	}
}; // NativeContact

class Molecule: public vector<Residue> {
	vector<NativeContact> natconts;
public:
	void add(Residue res) {
		push_back(res);
	}
	void add(Atom atom) {
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
	void addContact(NativeContact cont) {
		natconts.push_back(cont);
	}
	void getNatCont(float dfcontact) {
		for(int i=0; i<size(); i++) { for(int j=0; j<size(); j++) {
			if(at(i).getSeq() >= at(j).getSeq()-3) continue;
			float dist = at(i).distance(at(j));
			if (dist < dfcontact) {
				addContact(NativeContact(at(i), at(j), dist));
			}
		}}
	}
	void getBonds() {
		for(int i=0; i<size()-1; i++) {
			cout << setw(7) << at(i).getSeq() << ' ';
			cout << setw(7) << at(i+1).getSeq() << ' ';
			cout << setw(8) << at(i).getCA().distance(at(i+1).getCA()) << '\n';
		}
	}
	void getAngles() {
		for(int i=0; i<size()-2; i++) {
			cout << setw(7) << at(i).getSeq() << ' ';
			cout << setw(7) << at(i+1).getSeq() << ' ';
			cout << setw(7) << at(i+2).getSeq() << ' ';
			cout << setw(8) << at(i+1).getCA().angle(at(i).getCA(), at(i+2).getCA()) << '\n';
		}
	}
}; // Molecule

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

	Molecule mol;
	while ( getline(fin, line) ) {
		if (line.substr(0,4).compare("ATOM") == 0) {
			Atom atom(line);
			mol.add(atom);
		}
	}

	cout << "# structure" << '\n';
	for (int i=0; i<mol.size(); i++) {
		mol.at(i).getCA().show();
	}
	cout << "# bonds" << '\n';
	mol.getBonds();
	cout << "# angles" << '\n';
	mol.getAngles();
	cout << "# native contacts" << '\n';
	mol.getNatCont(6.5);
	return 0;
}

