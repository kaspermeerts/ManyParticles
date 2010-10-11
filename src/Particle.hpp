#ifndef _PARTICLE_H_
#define _PARTICLE_H_

struct vec3
{
	float x, y, z;
	vec3(float a, float b, float c);
	vec3 operator+(const vec3 &v) const;
	vec3 operator-(const vec3 &v) const;
	float operator*(const vec3 &v) const;
	vec3 operator*(float v) const;
	vec3 operator/(float v) const;
	float length(void) const;
};

#endif
