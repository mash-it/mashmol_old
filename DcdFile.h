#include <fstream>
#include <vector>
class DcdFile
{
	int nframes, istart, nstep_save, nsteps, nunits, dcdVersion, natoms;
	float delta;

	std::string credit;
	std::ofstream fout;

public:
	DcdFile();
	void open(std::string filename);
	void setNAtoms(int);
	void writeFrame(std::vector<float> rx, std::vector<float> ry, std::vector<float> rz);
};
