#include "DcdFile.h"

DcdFile::DcdFile() {
	nframes = 100;
	istart = 0;
	nstep_save = 1;
	nsteps = nframes * nstep_save;
	nunits = 1;
	delta = 0.1;
	dcdVersion = 24;
	credit = "==== mashmol - Coarse Grained MD Simulation program, clone of CafeMol       ====";
}

void DcdFile::open(std::string filename) {
	int dummy;
	fout.open(filename, std::ios::out | std::ios::binary);
	if (!fout) {
		throw "cannot open dcd file ";
	}

	// 1st block
	int blocksize = 84;

	fout.write((char*)&blocksize, sizeof(int));
	fout.write("CORD", sizeof(char)*4);
	fout.write((char*)&nframes, sizeof(int));
	fout.write((char*)&istart, sizeof(int));
	fout.write((char*)&nstep_save, sizeof(int));
	fout.write((char*)&nsteps, sizeof(int));
	fout.write((char*)&nunits, sizeof(int));

	for(int i=0; i<4; i++) {
		dummy = 0;
		fout.write((char*)&dummy, sizeof(int));
	}
	fout.write((char*)&delta, sizeof(float));
	for(int i=0; i<9; i++) {
		dummy = 0;
		fout.write((char*)&dummy, sizeof(int));
	}
	fout.write((char*)&dcdVersion, sizeof(int));
	fout.write((char*)&blocksize, sizeof(int));
	
	// 2nd block
	blocksize = 84;
	fout.write((char*)&blocksize, sizeof(int));
	dummy = 1;
	fout.write((char*)&dummy, sizeof(int));
	fout << credit;
	fout.write((char*)&blocksize, sizeof(int));

	// 3rd block
	blocksize = 4;
	fout.write((char*)&blocksize, sizeof(int));
	fout.write((char*)&natoms, sizeof(int));
	fout.write((char*)&blocksize, sizeof(int));


}
void DcdFile::setNAtoms(int n) { natoms = n; }

void DcdFile::writeFrame(std::vector<float> rx, std::vector<float> ry, std::vector<float> rz) {

	int blocksize = rx.size() * sizeof(float) - 4;

	fout.write((char*)&blocksize, sizeof(int));
	for(int i=1; i<rx.size(); i++) {
		fout.write((char*)&rx[i], sizeof(float));
	}
	fout.write((char*)&blocksize, sizeof(int));

	fout.write((char*)&blocksize, sizeof(int));
	for(int i=1; i<rx.size(); i++) {
		fout.write((char*)&ry[i], sizeof(float));
	}
	fout.write((char*)&blocksize, sizeof(int));

	fout.write((char*)&blocksize, sizeof(int));
	for(int i=1; i<rx.size(); i++) {
		fout.write((char*)&rz[i], sizeof(float));
	}
	fout.write((char*)&blocksize, sizeof(int));
}
