#ifndef _VMATH_H_
#define _VMATH_H_

#include <math.h>

typedef struct Vec3
{
	float x, y, z;
} Vec3;

/* In header for inlining */

void add(const Vec3 *a, const Vec3 *b, Vec3 *dest)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

void sub(const Vec3 *a, const Vec3 *b, Vec3 *dest)
{
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
	dest->z = a->z - b->z;
}

void scale(const Vec3 *v, float lambda, Vec3 *dest)
{
	dest->x = lambda * v->x;
	dest->y = lambda * v->y;
	dest->z = lambda * v->z;
}

float dot(const Vec3 *v, const Vec3 *w)
{
	return v->x * w->x + v->y * w->y + v->z * w->z;
}

float length2(const Vec3 *v)
{
	return dot(v, v);
}

float length(const Vec3 *v)
{
	return sqrt(length2(v));
}

#endif
