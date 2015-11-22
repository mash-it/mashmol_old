#include <iostream>
#include <iomanip>
#include <cmath>
#include "MdSystem.h"

MdSystem::MdSystem() {
	dt = 1;
	k_stretch = 100.0;
	k_bend = 20.0;
	k_dihed1 = 1.0;
	k_dihed3 = 0.5;
}

void MdSystem::setNAtoms(int n) {
	natoms = n;
	mdIndex.resize(n);
	rx.resize(n);
	ry.resize(n);
	rz.resize(n);
	vx.resize(n);
	vy.resize(n);
	vz.resize(n);
	mass.resize(n);
	for (int i=0; i<n; i++) {
		mass[i] = 1;
	}
}
void MdSystem::setNForces(int s, int a, int d, int c) {
	stretch.resize(s);
	bend.resize(a);
	dihedral.resize(d);
	contact.resize(c);
}
void MdSystem::setAtom(int i, int mdi, float x, float y, float z) {
	mdIndex[i] = mdi;
	rx[i] = x;
	ry[i] = y;
	rz[i] = z;
}
void MdSystem::setStretch(int i, int n1, int n2, float length) {
	stretch[i].n1 = n1;
	stretch[i].n2 = n2;
	stretch[i].length = length;
}

void MdSystem::applyStretches() {
	for(int i=0; i<stretch.size(); i++) {
	}
}

void MdSystem::setBend(int i, int n1, int n2, int n3, float angle) {
	bend[i].n1 = n1;
	bend[i].n2 = n2;
	bend[i].n3 = n3;
	bend[i].angle = angle;
}
void MdSystem::setDihed(int i, int n1, int n2, int n3, int n4, float d) {
	dihedral[i].n1 = n1;
	dihedral[i].n2 = n2;
	dihedral[i].n3 = n3;
	dihedral[i].n4 = n4;
	dihedral[i].dangle = d;
}
void MdSystem::setContact(int i, int n1, int n2, float distance) {
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

void MdSystem::step() {
	// apply velocity to position 
	for (int i=0; i<natoms; i++) {
		rx[i] += vx[i] * dt;
		ry[i] += vy[i] * dt;
		rz[i] += vz[i] * dt;
	}

	// apply force to velocity
	applyStretches();

	// (debug) show state
	using namespace std;
	cout << setw(12) << mdIndex[0] << ' ';
	cout << "rx=" << setw(12) << rx[0] << ' ';
	cout << "vx=" << setw(12) << vx[0] << ' ';
	cout << '\n';
}
