#include <cmath>
#include "Particle.hpp"

vec3::vec3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
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

float vec3::length() const
{
	return sqrt(x*x + y*y + z*z);
}
