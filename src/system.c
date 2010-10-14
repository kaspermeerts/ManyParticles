#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vmath.h"
#include "system.h"

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
	int numParticles;
} Config;

bool collides(const Particle *p);
PartList **boxFromParticle(const Particle *p);
PartList **boxAddress(int nx, int ny, int nz);
bool collideWith(const Particle *p, const PartList *ps);

World world;
Config config;

PartList **boxFromParticle(const Particle *p)
{
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	return boxAddress(nx, ny, nz);
}

PartList **boxAddress(int nx, int ny, int nz)
{
	return world.grid + nx * config.numBox * config.numBox + ny * config.numBox + nz;
}

bool collides(const Particle *p)
{
	int ix, iy, iz;
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	if (collideWith(p, *boxAddress(nx, ny, nz)))
		return true;

	for (ix = -1; ix <= 1; ix++)
	for (iy = -1; iy <= 1; iy++)
	for (iz = -1; iz <= 1; iz++)
	{
		if (ix == 0 && iy == 0 && iz == 0)
			continue;

		if (nx + ix < 0 || nx + ix >= config.numBox) continue;
		if (ny + iy < 0 || ny + iy >= config.numBox) continue;
		if (nz + iz < 0 || nz + iz >= config.numBox) continue;

		if (collideWith(p, *boxAddress(nx+ix, ny+iy, nz+iz)))
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
		op = other->particle;
		sub(&p->pos, &op->pos, &diff);
		if (length2(&diff) < (p->r + op->r)*(p->r + op->r))
			return true;
	}

	return false;
}

/* Precondition: config MUST be valid
 * Allocates and fills */
bool fillWorld()
{
	int nb = config.numBox;
	int i;
	Particle *ps;
	PartList *nodes;
	PartList **box;
	float worldSize = config.numBox * config.boxSize;

	world.parts = calloc(config.numParticles, sizeof(Particle));
	ps = world.parts;
	if (world.parts == NULL)
	{
		MEM_ERROR(config.numParticles * sizeof(Particle));
		return false;
	}

	nodes = calloc(config.numParticles, sizeof(*nodes));
	
	world.grid = calloc(nb * nb * nb, sizeof(*(world.grid)));

	for (i = 0; i < config.numParticles; i++)
	{
		do
		{
			ps[i].pos.x = rand() * worldSize / RAND_MAX;
			ps[i].pos.y = rand() * worldSize / RAND_MAX;
			ps[i].pos.z = rand() * worldSize / RAND_MAX;
		} while (collides(&ps[i]));

		nodes[i].particle = &ps[i];
		
		box = boxFromParticle(&ps[i]);
		nodes[i].next = *box;
		*box = &nodes[i];
	}

	return true;
}
		
int main(int argc, char **argv)
{
	config.boxSize = 100.;
	config.numBox = 10;
	config.numParticles = 1000;

	fillWorld();

	return 0;
}




