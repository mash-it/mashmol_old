#include <iostream>
#include <fstream>

#include "util.h"

using namespace std;

int main(int argc, char *argv[]) {
	std::string atomPath = "atom.dat";
	std::string ninfoPath = "ninfo.dat";
	std::string confPath = "conf.dat";

	// open atom file
	std::ifstream atomFile(atomPath.c_str());
	if (atomFile.fail())
	{
		std::cerr << "failed to open " << atomPath << std::endl;
		return 1;
	}

	std::string line;
	std::string record;
	int natoms = 0;
	while (getline(atomFile, line)) {
		record = util::removeSpaces(line.substr(0,8));
		if (record == "ATOM") {
			natoms++;
		}
	}
	cout << natoms << endl;
}
