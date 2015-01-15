
#include"vector3f.h"
#include<math.h>
#include <iostream>

using namespace std;

vector3f::vector3f(){}

vector3f::vector3f(float ix, float iy, float iz): x(ix), y(iy), z(iz){}

vector3f::~vector3f(){}

vector3f vector3f::operator+(const vector3f& right){
	vector3f rvec;
	rvec.x = x + right.x;
	rvec.y = y + right.y;
	rvec.z = z + right.z;
	
	return rvec;
}

vector3f vector3f::operator-(const vector3f& right){
	vector3f rvec;
	rvec.x = x - right.x;
	rvec.y = y - right.y;
	rvec.z = z - right.z;
	
	return rvec;
}

vector3f& vector3f::operator=(const vector3f& right){
	x = right.x;
	y = right.y;
	z = right.z;

	return *this;
}

vector3f vector3f::cross(const vector3f& right){
	vector3f rvec;
	rvec.x = y * right.z - z * right.y;
	rvec.y = z * right.x - x * right.z;
	rvec.z = x * right.y - y * right.x;
	
	return rvec;
}
vector3f vector3f::normalize(){
	float mag;
	
	mag = sqrt(x*x + y*y + z*z);
	x = x/mag;
	y = y/mag;
	z = z/mag;

	return *this;
}

vector3f vector3f::scale(float s){
	x = s * x;
	y = s * y;
	z = s * z;

	return *this;
}

void vector3f::printVec(){
	cout << "(" << x << "," << y << "," << z << ")" << endl;
}