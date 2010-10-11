#include "Particle.hpp"

vec3::vec3(float a, float b, float c)
{
	x = a;
	y = b;
	z = c;
}
	
vec3 vec3::operator+(const vec3 &v)
{
	float a, b, c;

	a = x + v.x;
	b = y + v.y;
	c = z + v.z;

	return vec3(a, b, c);
}
