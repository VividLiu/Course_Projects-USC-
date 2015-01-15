#ifndef _VECTOR3F_H
#define _VECTOR3F_H

class vector3f{
	public:
		float x;
		float y;
		float z;
		
	//constructor & destructor
	vector3f();
	
	vector3f(float ix, float iy, float iz);
	
	~vector3f();
	
	//operand overloading
	vector3f operator+(const vector3f& right);
	vector3f operator-(const vector3f& right);
	vector3f& operator=(const vector3f& right);
	
	//member function
	vector3f cross(const vector3f& right);
	vector3f normalize();
	vector3f scale(float s);
	void printVec();
	
};
#endif








