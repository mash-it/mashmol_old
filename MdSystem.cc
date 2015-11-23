#include <iostream>
#include <iomanip>
#include <cmath>
#include "MdSystem.h"

using namespace std;

MdSystem::MdSystem() {
	dt = 0.001;
	k_stretch = 100.0;
	k_bend = 20.0;
	k_dihed1 = 1.0;
	k_dihed3 = 0.5;
}

void MdSystem::setNAtoms(int n) {
	maxMdIndex = n; // max value of mdIndex
	n++; // avoid 0-th atom
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
	dcd.setNAtoms(n-1);
}
void MdSystem::setNForces(int s, int a, int d, int c) {
	stretch.resize(s);
	bend.resize(a);
	dihedral.resize(d);
	contact.resize(c);
}
void MdSystem::setAtom(int i, float x, float y, float z) {
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
	float drx, dry, drz, dr; // difference of position
	float f, fx, fy, fz; // force
	int n1, n2;
	for(int i=0; i<stretch.size(); i++) {
		n1 = stretch[i].n1;
		n2 = stretch[i].n2;
		drx = rx[n1] - rx[n2];
		dry = ry[n1] - ry[n2];
		drz = rz[n1] - rz[n2];
		dr  = sqrt(drx*drx + dry*dry + drz*drz);
		f   = -k_stretch * (dr - stretch[i].length);
		fx  = f * drx / dr;
		fy  = f * dry / dr;
		fz  = f * drz / dr;
		vx[n1] += dt * fx / mass[n1];
		vy[n1] += dt * fy / mass[n1];
		vz[n1] += dt * fz / mass[n1];
		vx[n2] -= dt * fx / mass[n2];
		vy[n2] -= dt * fy / mass[n2];
		vz[n2] -= dt * fz / mass[n2];
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
	for (int i=1; i<=maxMdIndex; i++) {
		coef = sqrt(tempk * BOLTZMANN / mass[i]);
		vx[i] = coef * gaussian(unirandom); 
		vy[i] = coef * gaussian(unirandom); 
		vz[i] = coef * gaussian(unirandom); 
	}
}

void MdSystem::step() {
	// apply velocity to position 
	for (int i=1; i<=maxMdIndex; i++) {
		rx[i] += vx[i] * dt;
		ry[i] += vy[i] * dt;
		rz[i] += vz[i] * dt;
	}

	// apply force to velocity
	applyStretches();

	// (debug) show state
	cout << setw(12) << rx[1] << ' ';
	cout << setw(12) << vx[1] << ' ';
	cout << '\n';

	dcd.writeFrame(rx, ry, rz);
}

void MdSystem::openDcd(std::string filename) {
	dcd.open(filename);
	dcd.setNAtoms(maxMdIndex);
}

