#ifndef _PARTICLE_H_
#define _PARTICLE_H_

struct vec3
{
	float x, y, z;
	vec3 operator+(const struct vec3 &v);
	vec3(float a, float b, float c);
};

#endif
