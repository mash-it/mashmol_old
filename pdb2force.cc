#include <iostream>
#include <fstream> // ifstream
#include <iomanip> // setw, setprecision
#include <unistd.h>
#include <vector>

#include "Molecule.h"

int main(int argc, char *argv[]) {
	std::string inpfile = "input.pdb";
	std::string atomfile = "atom.dat";
	std::string forcefile = "force.dat";

	int opt;
	while ((opt = getopt(argc, argv, "f:")) != -1) {
		switch(opt) {
			case 'f':
				inpfile = optarg;
		}
	}
	std::cout << "Read " << inpfile << '\n';

	// open pdb file
	std::ifstream fin(inpfile.c_str());

	if (fin.fail())
	{
		std::cerr << "failed to open " << inpfile << std::endl;
		return 1;
	}

	// read pdb file
	std::string line;

	Molecule mol;
	int imd=1; // index for md
	while ( getline(fin, line) ) {
		if (line.substr(0,4).compare("ATOM") == 0) {
			Atom atom(line);
			if (atom.getName() == "CA") {
 				atom.mdIndex = imd;
				imd++;
			}
			mol.add(atom);
		}
	}

	std::cout << "Write " << atomfile << " and " << forcefile << '\n';
	
	// write atom file
	std::ofstream afout(atomfile.c_str());
	
	afout << "# structure" << '\n';
	afout << mol.writeAtoms();
	afout.close();
	
	// write force file
	std::ofstream nfout(forcefile.c_str());
	
	nfout << "# stretch" << '\n';
	nfout << mol.writeStretches();
	nfout << "# bend" << '\n';
	nfout << mol.writeBends();
	nfout << "# dihedral" << '\n';
	nfout << mol.writeDihedral();
	nfout << "# native contacts" << '\n';
	nfout << mol.writeNatCont(6.5);
	
	nfout.close();


	return 0;
}

