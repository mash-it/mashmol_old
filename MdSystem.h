#include <vector>
#include "constants.h"
#include "random.h"
#include "DcdFile.h"

struct Stretch
{
	int n1, n2;
	float length; // bond length
};

struct Bend
{
	int n1, n2, n3;
	float angle; // bond angle
};

struct Dihedral
{
	int n1, n2, n3, n4;
	float dangle; // dihedral angle
};

struct Contact
{
	int n1, n2;
	float distance;
};

class MdSystem
{
	std::vector<int> mdIndex;
	std::vector<float> mass;

	// phase space
	std::vector<float> rx;
	std::vector<float> ry;
	std::vector<float> rz;
	std::vector<float> vx;
	std::vector<float> vy;
	std::vector<float> vz;

	// forces
	std::vector<Stretch> stretch;
	std::vector<Bend> bend;
	std::vector<Dihedral> dihedral;
	std::vector<Contact> contact;

	// system information
	int natoms;
	float dt;	// time step
	float k_stretch; // spring constant for stretching
	float k_bend; // ... for bending
	float k_dihed1; // ... for bending
	float k_dihed3; // ... for bending

	DcdFile dcd;

public:
	MdSystem();
	void setNAtoms(int natoms);
	void setNForces(int s, int a, int d, int c);
	void setAtom(int i, float x, float y, float z);
	void setStretch(int i, int n1, int n2, float length);
	void setBend(int i, int n1, int n2, int n3, float angle);
	void setDihed(int i, int n1, int n2, int n3, int n4, float d);
	void setContact(int i, int n1, int n2, float distance);
	void setIniVelo(float tempk);
	
	void step();
	void applyStretches();

	void openDcd(std::string filename);
	void writeDcdFrame();
};


