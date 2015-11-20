#ifndef _Vector3D_
#define _Vector3D_

#include <iostream>

class Vector3D
{
public:
	float x, y, z;
	void setCoord(float argx, float argy, float argz);
	Vector3D(float argx, float argy, float argz);
	Vector3D operator + (Vector3D other); 
	Vector3D operator - (Vector3D other);
	float operator * (Vector3D other);
	Vector3D cross (Vector3D t);
	float norm();
	static float angle(Vector3D l, Vector3D c, Vector3D r);
	static float dihedral(Vector3D a, Vector3D b, Vector3D c, Vector3D d);
	friend std::ostream& operator << (std::ostream& os, const Vector3D& v);
};

#endif
