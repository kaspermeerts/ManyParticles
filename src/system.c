#include <stdio.h>
#include <stdbool.h>
#include "vmath.h"

typedef struct particle
{
	Vec3 pos;
	Vec3 vel;
	float r;
} Particle;

typedef struct partList
{
	Particle *particle;
	struct partList *next;
} PartList;

typedef struct world
{
	PartList **grid;
	Particle *parts;
} World;

typedef struct config
{
	// The same for every dimension
	float boxSize;
	int numBox;
} Config;

bool collides(const Particle *p);
PartList *box(int nx, int ny, int nz);
bool collideWith(const Particle *p, const PartList *ps);

World world;
Config config;

PartList *box(int nx, int ny, int nz)
{
	return *world.grid + nx * config.numBox * config.numBox + ny * config.numBox + nz;
}

bool collides(const Particle *p)
{
	int ix, iy, iz;
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	if (collideWith(p, box(nx, ny, nz)))
		return true;

	for (ix = -1; ix <= 1; ix++)
	for (iy = -1; iy <= 1; iy++)
	for (iz = -1; iz <= 1; iz++)
	{
		if (ix == 0 && iy == 0 && iz == 0)
			continue;

		if (collideWith(p, box(nx+ix, ny+iy, nz+iz)))
			return true;
	}

	return false;
}


bool collideWith(const Particle *p, const PartList *ps)
{
	const PartList *other;
	Particle *op;
	Vec3 diff;

	for (other = ps; other; other = other->next)
	{
		op = &other->particle;
		sub(&p->pos, &op->pos, &diff);
		if (length2(&diff) < (p->r + op->r)*(p->r + op->r))
			return true;
	}

	return false;
}
