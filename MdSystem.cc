#include <cmath>
#include "MdSystem.h"

void MdSystem::setNAtoms(int natoms) {
	rx.resize(natoms);
	ry.resize(natoms);
	rz.resize(natoms);
	vx.resize(natoms);
	vy.resize(natoms);
	vz.resize(natoms);
	mass.resize(natoms);
	for (int i=0; i<natoms; i++) {
		mass[i] = 1;
	}
}
void MdSystem::setNForces(int s, int a, int d, int c) {
	stretch.resize(s);
	bend.resize(a);
	dihedral.resize(d);
	contact.resize(c);
}
void MdSystem::setCoord(int i, float x, float y, float z) {
	rx[i] = x;
	ry[i] = y;
	rz[i] = z;
}
void MdSystem::setStretches(int i, int n1, int n2, float length) {
	stretch[i].n1 = n1;
	stretch[i].n2 = n2;
	stretch[i].length = length;
}
void MdSystem::setBends(int i, int n1, int n2, int n3, float angle) {
	bend[i].n1 = n1;
	bend[i].n2 = n2;
	bend[i].n3 = n3;
	bend[i].angle = angle;
}
void MdSystem::setDiheds(int i, int n1, int n2, int n3, int n4, float d) {
	dihedral[i].n1 = n1;
	dihedral[i].n2 = n2;
	dihedral[i].n3 = n3;
	dihedral[i].n4 = n4;
	dihedral[i].dangle = d;
}
void MdSystem::setContacts(int i, int n1, int n2, float distance) {
	contact[i].n1 = n1;
	contact[i].n2 = n2;
	contact[i].distance = distance;
}
void MdSystem::setIniVelo(float tempk) {
	float coef;
	for (int i=0; i<vx.size(); i++) {
		coef = sqrt(tempk * BOLTZMANN / mass[i]);
		vx[i] = coef * gaussian(unirandom); 
		vy[i] = coef * gaussian(unirandom); 
		vz[i] = coef * gaussian(unirandom); 
	}
}
