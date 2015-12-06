#include <iostream>
#include <iomanip>
#include <cmath>
#include "MdSystem.h"
#include "constants.h"

using namespace std;

MdSystem::MdSystem() {
	dt = 0.001;
	k_stretch = 100.0;
	k_bend = 10.0;
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
	int i, j, k; // index of three atoms for one angle. k=j+1=i+2
	float theta, thetaNative; // (native) bond angle
	float cosTheta, sinTheta;
	float drijx, drijy, drijz, drkjx, drkjy, drkjz; // two bond vectors
	float drij2, drkj2; // square of distance
	float S, D, D2;
	float force;
	float force_ijx, force_ijy, force_ijz;
	float force_kjx, force_kjy, force_kjz;

	for (int ibend=0; ibend<bend.size(); ibend++) {
		i = bend[ibend].n1; j = bend[ibend].n2; k = bend[ibend].n3;
		thetaNative = bend[ibend].angle * pi / 180;
		drijx = rx[i]-rx[j]; drijy = ry[i]-ry[j]; drijz = rz[i]-rz[j];
		drkjx = rx[k]-rx[j]; drkjy = ry[k]-ry[j]; drkjz = rz[k]-rz[j];
		drij2 = drijx*drijx + drijy*drijy + drijz*drijz;
		drkj2 = drkjx*drkjx + drkjy*drkjy + drkjz*drkjz;

		S = drijx*drkjx + drijy*drkjy + drijz*drkjz;
		D2 = drij2 * drkj2;
		D = sqrt(D2);

		cosTheta = S/D;
		// avoid imaginary sinTheta
		if (cosTheta > 1) cosTheta = 1;
		if (cosTheta < -1) cosTheta = -1;

		sinTheta = sqrt(1 - cosTheta*cosTheta);
		if (sinTheta < 0.001) sinTheta = 0.001;

		theta = acos(cosTheta);
		force = -k_bend * (theta - thetaNative) / (sinTheta * D);

		force_ijx = force * (drkjx - S/D2 * drijx * drkj2);
		force_ijy = force * (drkjy - S/D2 * drijy * drkj2);
		force_ijz = force * (drkjz - S/D2 * drijz * drkj2);

		force_kjx = force * (drijx - S/D2 * drkjx * drij2);
		force_kjy = force * (drijy - S/D2 * drkjy * drij2);
		force_kjz = force * (drijz - S/D2 * drkjz * drij2);

		vx[i] -= dt * force_ijx / mass[i];
		vy[i] -= dt * force_ijy / mass[i];
		vz[i] -= dt * force_ijz / mass[i];

		vx[j] += dt * (force_ijx + force_kjx) / mass[j];
		vy[j] += dt * (force_ijy + force_kjy) / mass[j];
		vz[j] += dt * (force_ijz + force_kjz) / mass[j];

		vx[k] -= dt * force_kjx / mass[k];
		vy[k] -= dt * force_kjy / mass[k];
		vz[k] -= dt * force_kjz / mass[k];
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

void MdSystem::setCenterZero() {

	// set center of mass position to zero
	float cmx = 0;
	float cmy = 0;
	float cmz = 0;
	for (int i=0; i<natoms; i++) {
		cmx += rx[i]; cmy += ry[i]; cmz += rz[i];
	}
	cmx /= natoms; cmy /= natoms; cmz /= natoms;

	for (int i=0; i<natoms; i++) {
		rx[i] -= cmx; ry[i] -= cmy; rz[i] -= cmz;
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

	setCenterZero();

}

void MdSystem::openDcd(std::string filename, int nstep, int nstepSave) {
	dcd.setMeta(nstep, nstepSave);
	dcd.open(filename);
}

void MdSystem::writeDcdFrame() {
	dcd.writeFrame(rx, ry, rz);
}

