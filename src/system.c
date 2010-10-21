#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "vmath.h"
#include "system.h"

typedef struct particle
{
	Vec3 pos;
	Vec3 vel;
	float r;
	struct particle *prev, *next;
} Particle;

typedef struct world
{
	Particle **grid;
	Particle *parts;
} World;

typedef struct config
{
	/* The same for every dimension */
	float boxSize;
	int numBox;
	int numParticles;
	float radius;
} Config;

typedef struct stats
{
	int misses;
} Stats;

int main(int argc, char ** argv);
bool collides(const Particle *p);
Particle **boxFromParticle(const Particle *p);
Particle **boxFromIndex(int nx, int ny, int nz);
bool collideWith(const Particle *p, const Particle *ps);
bool fillWorld(void);
void freeWorld(void);
void dumpWorld(void);
void sanityCheck(void);
void densityDump(void);

World world;
Config config;
Stats stats;

Particle **boxFromParticle(const Particle *p)
{
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	return boxFromIndex(nx, ny, nz);
}

Particle **boxFromIndex(int nx, int ny, int nz)
{
	return world.grid + nx * config.numBox * config.numBox + 
			ny * config.numBox + nz;
}

bool collides(const Particle *p)
{
	int ix, iy, iz;
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	if (collideWith(p, *boxFromIndex(nx, ny, nz)))
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

		if (collideWith(p, *boxFromIndex(nx+ix, ny+iy, nz+iz)))
			return true;
	}

	return false;
}

/* p is the particle we're checking, ps is the first particle in the box */
bool collideWith(const Particle *p, const Particle *ps)
{
	const Particle *other = ps;
	Vec3 diff;
	
	if (ps == NULL)
		return false;

	do
	{
		if (p == other)
			continue;

		sub(&p->pos, &other->pos, &diff);
		if (length2(&diff) < (p->r + other->r)*(p->r + other->r))
			return true;

		other = other -> next;
	} while (other != ps);

	return false;
}

/* Precondition: config MUST be valid
 * Allocates and fills */
bool fillWorld()
{
	int nb = config.numBox;
	int i;
	Particle *ps;
	Particle **box;
	float worldSize = config.numBox * config.boxSize;

	world.parts = calloc(config.numParticles, sizeof(Particle));
	ps = world.parts;
	if (world.parts == NULL)
	{
		MEM_ERROR(config.numParticles * sizeof(Particle));
		return false;
	}

	world.grid = calloc(nb * nb * nb, sizeof(*(world.grid)));

	for (i = 0; i < config.numParticles; i++)
	{
		ps[i].r = config.radius;
		stats.misses--;
		do
		{
			stats.misses++;
			ps[i].pos.x = rand() * worldSize / RAND_MAX;
			ps[i].pos.y = rand() * worldSize / RAND_MAX;
			ps[i].pos.z = rand() * worldSize / RAND_MAX;
		} while (collides(&ps[i]));

		box = boxFromParticle(&ps[i]);
		if (*box == NULL)
		{
			*box = &ps[i];
			ps[i].prev = &ps[i];
			ps[i].next = &ps[i];
		} else
		{
			ps[i].next = *box;
			ps[i].prev = (*box)->prev;
			ps[i].prev->next = &ps[i];
			ps[i].next->prev = &ps[i];
			*box = &ps[i];
		}
	}

	return true;
}

void freeWorld()
{
	free(world.grid);
	free(world.parts);

	return;
}

void dumpWorld()
{
	int i;

	for (i = 0; i < config.numParticles; i++)
	{
		const Particle *p = &world.parts[i];
		printVector(&p->pos);
	}

	return;
}

void densityDump(void)
{
	int i;
	int n = config.numBox / 2;
	Particle **box = boxFromIndex(n, n, n);
	Particle *p = *box;

	if (p == NULL)
		return;

	for(i = 0; i < config.numParticles; i++)
	{
		Particle *other = &world.parts[i];
		if (p == other)
			continue;

		printf("%f\n", distance(&p->pos, &other->pos));
	}

	return;
}

void sanityCheck()
{
	int i, j;
	const Particle *p1, *p2;

	for (i = 0; i < config.numParticles; i++)
	{
		p1 = &world.parts[i];
		for (j = i + 1; j < config.numParticles; j++)
		{
			float d;
			p2 = &world.parts[j];
			d = distance(&p1->pos, &p2->pos);
			if (d < p1->r + p2->r)
				fprintf(stderr, "%f %f %f PROBLEM?\n", 
						d, p1->r, p2->r);
		}
	}


	return;
}

int main(int argc, char **argv)
{

	if (argc < 5)
	{
		fprintf(stderr, "This needs 5 arguments\n");
		return 1;
	}

	config.boxSize = atof(argv[1]);
	config.numBox = atoi(argv[2]);
	config.numParticles = atoi(argv[3]);
	config.radius = atof(argv[4]);

	stats.misses = 0;

	srand(time(NULL));

	fillWorld();

	/*dumpWorld();*/

	densityDump();

	/*sanityCheck();*/

	freeWorld();

	return 0;
}




