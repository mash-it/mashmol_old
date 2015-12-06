#include <iostream>
#include <fstream>
#include "util.h"
#include "constants.h"
#include "MdSystem.h"

// set global random generator
#include <random>
std::mt19937 unirandom;
std::normal_distribution<> gaussian;


int main(int argc, char *argv[]) {
	std::string atomPath = "atom.dat";
	std::string forcePath = "force.dat";
	std::string protPath = "protocol.dat";
	std::string bufferLine, record;
	MdSystem md;

	// read protocol while
	int nstep, nstepSave, nrandSeed;
	std::ifstream protFile(protPath.c_str());
	if (!protFile) {
		std::cerr << "cannot open " << protPath << std::endl;
		return 1;
	}
	while (std::getline(protFile, bufferLine)) {
		record = util::removeSpaces(bufferLine.substr(0,8));
		if (record == "NSTEP") nstep = stoi(bufferLine.substr(8,8));
		if (record == "NSAVE") nstepSave = stoi(bufferLine.substr(8,8));
		if (record == "RANDSEED") nrandSeed = stoi(bufferLine.substr(8,8));
	}
	unirandom.seed(nrandSeed);

	std::cout << "NSTEP: " << nstep << ", ";
	std::cout << "NSAVE: " << nstepSave << '\n';


	// read atom file
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
	std::cout << "ATOM : " << natoms << std::endl;
	atomFile.clear();

	md.setNAtoms(natoms);

	// read atom coordinate
	atomFile.seekg(0, std::ios_base::beg);
	int iatoms=0;
	int mdIndex;
	float x,y,z;
	while (std::getline(atomFile, bufferLine)) {;
		record = util::removeSpaces(bufferLine.substr(0,8));
		if (record == "ATOM") {
			mdIndex = stoi(bufferLine.substr(8,8));
			if (mdIndex != iatoms) {
				throw "mdIndex is not sequenced";
			}
			x = stof(bufferLine.substr(16,8));
			y = stof(bufferLine.substr(24,8));
			z = stof(bufferLine.substr(32,8));
			md.setAtom(mdIndex, x, y, z);
			iatoms++;
		}
	}
	atomFile.close();

	// read force file
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
	std::cout << nstretchs << " " << nbends << " " << ndiheds << " " << nconts << std::endl;
	forceFile.clear();

	forceFile.seekg(0, std::ios_base::beg);
	int istretchs=0, ibends=0, idiheds=0, iconts=0;
	int n1, n2, n3, n4;
	float val;

	while (std::getline(forceFile, bufferLine)) {;
		record = util::removeSpaces(bufferLine.substr(0,8));
		if (record == "STRETCH") {
			n1 = stoi(bufferLine.substr( 8,8));
			n2 = stoi(bufferLine.substr(16,8));
			val= stof(bufferLine.substr(24,8));
			md.setStretch(istretchs, n1, n2, val);
			istretchs++;
		}
		if (record == "BEND") {
			n1 = stoi(bufferLine.substr( 8,8));
			n2 = stoi(bufferLine.substr(16,8));
			n3 = stoi(bufferLine.substr(24,8));
			val= stof(bufferLine.substr(32,8));
			md.setBend(ibends, n1, n2, n3, val);
			ibends++;
		}
		if (record == "DIHED") {
			n1 = stoi(bufferLine.substr( 8,8));
			n2 = stoi(bufferLine.substr(16,8));
			n3 = stoi(bufferLine.substr(24,8));
			n3 = stoi(bufferLine.substr(24,8));
			n4 = stoi(bufferLine.substr(32,8));
			val= stof(bufferLine.substr(40,8));
			md.setDihed(idiheds, n1, n2, n3, n4, val);
			idiheds++;
		}
		if (record == "CONTACT") {
			n1 = stoi(bufferLine.substr( 8,8));
			n2 = stoi(bufferLine.substr(16,8));
			val= stof(bufferLine.substr(24,8));
			md.setContact(iconts, n1, n2, val);
			iconts++;
		}
	} // while

	std::cout << "STRETCH: " << istretchs << ", ";
	std::cout << "BEND: " << ibends << ", ";
	std::cout << "DIHEDRAL: " << idiheds << ", ";
	std::cout << "CONTACT: " << iconts << std::endl;
	forceFile.close();

	md.openDcd("test.dcd", nstep, nstepSave);

	md.setIniVelo(300);

	md.writeDcdFrame();
	int nframes = 1;
	for (int t=0; t<nstep; t++){
		md.step();
		if (t % nstepSave == 0) {
			md.writeDcdFrame();
			nframes++;
		}
	}
	std::cout << "nframes: " << nframes << std::endl;
}
