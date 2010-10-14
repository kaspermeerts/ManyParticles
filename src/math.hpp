#ifndef _MATH_H_
#define _MATH_H_

#include <iostream>

struct vec3
{
	float x, y, z;
	vec3(float a, float b, float c);
	vec3& operator=(const vec3 &v);
	vec3 operator+(const vec3 &v) const;
	vec3 operator-(const vec3 &v) const;
	float operator*(const vec3 &v) const;
	vec3 operator*(float v) const;
	vec3 operator/(float v) const;
	float length(void) const;
	float length2(void) const;
	static float length(const vec3 &v);
	static float length2(const vec3 &v);
};

std::ostream& operator<<(std::ostream& out, const vec3 &r);

#endif
