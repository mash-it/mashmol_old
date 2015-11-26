#include <iostream>
#include <iomanip>
#include <cmath>
#include "MdSystem.h"
#include "constants.h"

using namespace std;

MdSystem::MdSystem() {
	dt = 0.001;
	k_stretch = 100.0;
	k_bend = 20.0;
	k_dihed1 = 1.0;
	k_dihed3 = 0.5;
}

void MdSystem::setNAtoms(int n) {
	natoms = n; // max value of mdIndex
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
	dcd.setNAtoms(n);
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
	int n1, n2; // number of particle
	for(int i=0; i<stretch.size(); i++) {
		// mdIndex of two atoms bonded
		n1 = stretch[i].n1;
		n2 = stretch[i].n2;
		// difference of position
		drx = rx[n1] - rx[n2];
		dry = ry[n1] - ry[n2];
		drz = rz[n1] - rz[n2];
		dr  = sqrt(drx*drx + dry*dry + drz*drz);
		// stretching force (2 is from differentiation of potential)
		f   = -2 * k_stretch * (dr - stretch[i].length);
		fx  = f * drx / dr;
		fy  = f * dry / dr;
		fz  = f * drz / dr;
		// acceleration
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

void MdSystem::applyBends() {
	// "Numerical Simulation in Molecular Dynamics: Numerics, Algoriths, Parallelization, Applications"
	// M Griebel, S Knapek, G Zumbusch, Springer 2007, TJ Barth et al, Springer, p188

	int n1, n2, n3; // mdIndex of atom
	float ax, ay, az, bx, by, bz;  // bond vector (a: n2-n1, b: n2-n3)
	float caa, cbb; // dot product of two vectors
	float theta, natTheta, sinTheta, cosTheta; // sine and cosine of bond angle
	float force_common; // common prefix
	float force_ax, force_ay, force_az, force_bx, force_by, force_bz;
	float D, D2; // sqrt(caa * cbb) and its square
	float S; // dot product of a and b

	for (int i=0; i<bend.size(); i++) {
		n1 = bend[i].n1; n2 = bend[i].n2; n3 = bend[i].n3;
		natTheta = bend[i].angle * pi / 180;

		ax = rx[n2]-rx[n1]; ay = ry[n2]-ry[n1]; az = rz[n2]-rz[n1];
		bx = rx[n2]-rx[n3]; by = ry[n2]-ry[n3]; bz = rz[n2]-rz[n3];
		caa = ax*ax + ay*ay + az*az;
		cbb = bx*bx + by*by + bz*bz;
		S = ax*bx + ay*by + az*bz;
		D2 = caa * cbb;
		D = sqrt(D2);

		cosTheta = S/D;
		theta = acos(cosTheta);
		sinTheta = sqrt(1-cosTheta*cosTheta);
		if (sinTheta < 0.001) sinTheta = 0.001;

		force_common = -k_bend * (theta - natTheta) / sinTheta;
		force_common /= D;

		force_ax = force_common*(bx-S/D2*ax*cbb);
		force_ay = force_common*(by-S/D2*ay*cbb);
		force_az = force_common*(bz-S/D2*az*cbb);
		force_bx = force_common*(ax-S/D2*bx*caa);
		force_by = force_common*(ay-S/D2*by*caa);
		force_bz = force_common*(az-S/D2*bz*caa);

		vx[n1] -= dt * force_ax / mass[n1];
		vy[n1] -= dt * force_ay / mass[n1];
		vz[n1] -= dt * force_az / mass[n1];

		vx[n1] += dt * (force_ax - force_bx) / mass[n1];
		vy[n1] += dt * (force_ay - force_by) / mass[n1];
		vz[n1] += dt * (force_az - force_bz) / mass[n1];

		vx[n3] += dt * force_bx / mass[n3];
		vy[n3] += dt * force_by / mass[n3];
		vz[n3] += dt * force_bz / mass[n3];
	}
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
	for (int i=0; i<=natoms; i++) {
		coef = sqrt(tempk * BOLTZMANN / mass[i]);
		vx[i] = coef * gaussian(unirandom); 
		vy[i] = coef * gaussian(unirandom); 
		vz[i] = coef * gaussian(unirandom); 
	}
}

void MdSystem::step() {
	using namespace std;
	// apply velocity to position 
	for (int i=0; i<natoms; i++) {
		rx[i] += vx[i] * dt;
		ry[i] += vy[i] * dt;
		rz[i] += vz[i] * dt;
	}

	// apply force to velocity
	applyStretches();
	applyBends();
}

void MdSystem::openDcd(std::string filename, int nstep, int nstepSave) {
	dcd.setMeta(nstep, nstepSave);
	dcd.open(filename);
}

void MdSystem::writeDcdFrame() {
	dcd.writeFrame(rx, ry, rz);
}

