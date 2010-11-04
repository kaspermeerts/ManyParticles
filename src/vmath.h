/*
 * Many-particle simulation software
 * Copyright (C) 2010 Roald Frederickx and Kasper Meerts
 *
 * This program is released under the terms of the license contained
 * in the file LICENSE.
 */
#ifndef _VMATH_H_
#define _VMATH_H_

#include <stdio.h>
#include <math.h>

typedef struct Vec3
{
	float x, y, z;
} Vec3;

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

/* In header for inlining */

static __inline__ void printVector(const Vec3 *v);
static __inline__ void add(const Vec3 *a, const Vec3 *b, Vec3 *dest);
static __inline__ void sub(const Vec3 *a, const Vec3 *b, Vec3 *dest);
static __inline__ void scale(const Vec3 *v, float lambda, Vec3 *dest);
static __inline__ void normalize(const Vec3 *v, Vec3 *w);
static __inline__ float dot(const Vec3 *v, const Vec3 *w);
static __inline__ float length2(const Vec3 *v);
static __inline__ float length(const Vec3 *v);
static __inline__ float distance2(const Vec3 *a, const Vec3 *b);
static __inline__ float distance(const Vec3 *a, const Vec3 *b);

static __inline__ void printVector(const Vec3 *v)
{
	printf("%10f\t%10f\t%10f\t", v->x, v->y, v->z);
}

static __inline__ void add(const Vec3 *a, const Vec3 *b, Vec3 *dest)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
	dest->z = a->z + b->z;
}

static __inline__ void sub(const Vec3 *a, const Vec3 *b, Vec3 *dest)
{
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
	dest->z = a->z - b->z;
}

static __inline__ void scale(const Vec3 *v, float lambda, Vec3 *dest)
{
	dest->x = lambda * v->x;
	dest->y = lambda * v->y;
	dest->z = lambda * v->z;
}

static __inline__ void normalize(const Vec3 *v, Vec3 *w)
{
	float l = length(v);
	scale(v, 1/l, w);
}

static __inline__ float dot(const Vec3 *v, const Vec3 *w)
{
	return v->x * w->x + v->y * w->y + v->z * w->z;
}

static __inline__ float length2(const Vec3 *v)
{
	return dot(v, v);
}

static __inline__ float length(const Vec3 *v)
{
	return sqrt(length2(v));
}

static __inline__ float distance2(const Vec3 *a, const Vec3 *b)
{
	Vec3 c;

	sub(a, b, &c);
	return length2(&c);
}

static __inline__ float distance(const Vec3 *a, const Vec3 *b)
{
	return sqrt(distance2(a, b));
}
#endif
