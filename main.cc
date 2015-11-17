#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
using namespace std;

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
			cout << line << endl;
		}
	}
	return 0;
}

class Atom
{
	int serial;
	string name;
	string altLoc;
	string resName;
	string chainID;
	int resSeq;
	string iCode;
	float x;
	float y;
	float z;
	float occupancy;
	float tempFactor;
	string element;
	string charge;

public:
	Atom(string);
}

Atom::Atom(string line) {
};
