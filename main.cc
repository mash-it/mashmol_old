#include <iostream>
#include <fstream> // ifstream
#include <iomanip> // setw, setprecision
#include <unistd.h>
#include <vector>

#include "Molecule.h"

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
	mol.writeBonds(cout);
	cout << "# angles" << '\n';
	mol.writeAngles(cout);
	cout << "# dihedral" << '\n';
	mol.writeDihedral(cout);
	cout << "# native contacts" << '\n';
	mol.writeNatCont(cout, 6.5);
	return 0;
}

