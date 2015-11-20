#include <iostream>
#include <fstream> // ifstream
#include <iomanip> // setw, setprecision
#include <unistd.h>
#include <vector>

#include "Molecule.h"

using namespace std;

int main(int argc, char *argv[]) {
	string inpfile = "input.pdb";
	string atomfile = "atom.dat";
	string ninfofile = "ninfo.dat";

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

	
	// write atom file
	ofstream afout(atomfile.c_str());
	
	afout << "# structure" << '\n';
	for (int i=0; i<mol.size(); i++) {
		afout << mol.at(i).getCA().write();
	}
	afout.close();
	
	// write ninfo file
	ofstream nfout(ninfofile.c_str());
	
	nfout << "# bonds" << '\n';
	nfout << mol.writeBonds();
	nfout << "# angles" << '\n';
	nfout << mol.writeAngles();
	nfout << "# dihedral" << '\n';
	nfout << mol.writeDihedral();
	nfout << "# native contacts" << '\n';
	nfout << mol.writeNatCont(6.5);
	
	nfout.close();
	return 0;
}

