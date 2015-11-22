#include <vector>
#include "constants.h"
#include "random.h"

class Stretch
{
public:
	int n1, n2;
	float length; // bond length
};

class Bend
{
public:
	int n1, n2, n3;
	float angle; // bond angle
};

class Dihedral
{
public:
	int n1, n2, n3, n4;
	float dangle; // dihedral angle
};

class Contact
{
public:
	int n1, n2;
	float distance;
};

class MdSystem
{
	// position of particles
	std::vector<float> rx;
	std::vector<float> ry;
	std::vector<float> rz;
	std::vector<float> vx;
	std::vector<float> vy;
	std::vector<float> vz;
	std::vector<float> mass;
	std::vector<Stretch> stretch;
	std::vector<Bend> bend;
	std::vector<Dihedral> dihedral;
	std::vector<Contact> contact;

public:
	MdSystem() {}
	void setNAtoms(int natoms);
	void setNForces(int s, int a, int d, int c);
	void setCoord(int i, float x, float y, float z);
	void setStretches(int i, int n1, int n2, float length);
	void setBends(int i, int n1, int n2, int n3, float angle);
	void setDiheds(int i, int n1, int n2, int n3, int n4, float d);
	void setContacts(int i, int n1, int n2, float distance);
	void setIniVelo(float tempk);
};


