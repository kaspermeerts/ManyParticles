#include <cmath>
#include <iostream>
#include "math.hpp"

vec3::vec3(float a, float b, float c) : x(a), y(b), z(c) {}

vec3 &vec3::operator=(const vec3 &v)
{
	if(this == &v)
		return *this;
	
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

vec3 vec3::operator+(const vec3 &v) const
{
	float a, b, c;

	a = x + v.x;
	b = y + v.y;
	c = z + v.z;

	return vec3(a, b, c);
}

vec3 vec3::operator-(const vec3 &v) const
{
	const vec3 that = v * (-1);
	return that + (*this);
}

float vec3::operator*(const vec3 &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

vec3 vec3::operator*(float v) const
{
	return vec3(x*v, y*v, z*v);
}

vec3 vec3::operator/(float v) const
{
	return (*this * (1/v));
}

float vec3::length(void) const
{
	return sqrt(length2());
}

float vec3::length2(void) const
{
	return *this * *this;
}

float vec3::length(const vec3 &v)
{
	return sqrt(length2(v));
}

float vec3::length2(const vec3 &v)
{
	return v * v;
}

std::ostream& operator<<(std::ostream& s, const vec3& v)
{
	return s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
