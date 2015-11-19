#include "constants.h"
#include "Vector3D.h"
#include <cmath>

Vector3D::Vector3D(float argx, float argy, float argz)
{
	setCoord(argx, argy, argz);
}

void Vector3D::setCoord(float argx, float argy, float argz)
{
	x = argx;
	y = argy;
	z = argz;
}

Vector3D Vector3D::operator+ (Vector3D other)
{
	return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator- (Vector3D other)
{
	return Vector3D(x - other.x, y - other.y, z - other.z);
}

float Vector3D::operator * (Vector3D other)
{
	return this->x * other.x + this->y * other.y + this->z * other.z;
}

Vector3D Vector3D::cross (Vector3D t)
{
	return Vector3D(y*t.z - z*t.y, z*t.x - x*t.z, x*t.y - y*t.x);
}

float Vector3D::norm() 
{
	return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
}

float Vector3D::angle(Vector3D l, Vector3D c, Vector3D r) 
{
	Vector3D a = l - c;
	Vector3D b = r - c;
	return acos((a*b) / (a.norm()*b.norm())) * (180/pi);
}

float Vector3D::dihedral(Vector3D a, Vector3D b, Vector3D c, Vector3D d) 
{
	Vector3D ab = b-a;
	Vector3D bc = c-b;
	Vector3D cd = d-c;
	Vector3D abc = ab.cross(bc);
	Vector3D bcd = bc.cross(cd);
	Vector3D abcd = abc.cross(bcd);

	float dihed = acos((abc*bcd) / (abc.norm()*bcd.norm())) * (180/pi);
	if (bc * abcd > 0) {
		return dihed;
	} else {
		return -dihed;
	}
}

