#include <iostream>
#include <fstream> // ifstream
#include <iomanip> // setw, setprecision
#include <unistd.h>
#include <vector>

#include "Molecule.h"

int main(int argc, char *argv[]) {
	std::string inpfile = "input.pdb";
	std::string atomfile = "atom.dat";
	std::string ninfofile = "ninfo.dat";

	int opt;
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch(opt) {
			case 'f':
				inpfile = optarg;
		}
	}
	std::cout << "Read " << inpfile << '\n';

	std::ifstream fin(inpfile.c_str());

	// open pdb file
	if (fin.fail())
	{
		std::cerr << "failed to open " << inpfile << std::endl;
		return 1;
	}

	// read pdb file
	std::string line;

	Molecule mol;
	while ( getline(fin, line) ) {
		if (line.substr(0,4).compare("ATOM") == 0) {
			Atom atom(line);
			mol.add(atom);
		}
	}

	std::cout << "Write " << atomfile << " and " << ninfofile << '\n';
	
	// write atom file
	std::ofstream afout(atomfile.c_str());
	
	afout << "# structure" << '\n';
	for (int i=0; i<mol.size(); i++) {
		afout << mol.at(i).getCA().write();
	}
	afout.close();
	
	// write ninfo file
	std::ofstream nfout(ninfofile.c_str());
	
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

