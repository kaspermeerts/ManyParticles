#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vmath.h"
#include "system.h"

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

Particle **boxFromIndex(int ix, int iy, int iz)
{
	int nb = config.numBox;
	
	if (ix < 0)
		ix += nb;
	else if (ix >= nb)
		ix -= nb;

	if (iy < 0)
		iy += nb;
	else if (iy >= nb)
		iy -= nb;

	if (iz < 0)
		iz += nb;
	else if (iz >= nb)
		iz -= nb;

	return world.grid + ix*nb*nb + iy*nb + iz;
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
			/* To avoid hitting the exact boundary of the world
			 * shift 32 bits to 23 bits, the precision of a float
			 * mantissa */
			ps[i].pos.x = (rand() >> 9) * worldSize / 
					((RAND_MAX >> 9) + 1U);
			ps[i].pos.y = (rand() >> 9) * worldSize /
					((RAND_MAX >> 9) + 1U);
			ps[i].pos.z = (rand() >> 9) * worldSize / 
					((RAND_MAX >> 9) + 1U);
		} while (collides(&ps[i]));

		ps[i].vel.x = 1.0;
		ps[i].vel.y = 0.0;
		ps[i].vel.z = 0.0;

		box = boxFromParticle(&ps[i]);
		addToBox(&ps[i], box);
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

	printf("Checking sanity\n");

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

void stepWorld(void)
{
	Particle *p, *iterator;
	Particle **origBox, **newBox;
	float dt = config.timeStep;
	int nb = config.numBox;
	Vec3 dx;
	int i;

	for (i = 0; i < nb*nb*nb; i++)
	{
		printf("%d\n", i);
		origBox = &world.grid[i];
		if (*origBox == NULL)
			continue;

		iterator = *origBox;
		do
		{
			p = iterator;
			
			printVector(&p->pos);

			scale(&p->vel, dt, &dx);
			add(&p->pos, &dx, &p->pos);

			printVector(&p->pos);

			newBox = boxFromParticle(p);
			if (newBox != origBox)
				transferParticle(p, origBox, newBox);

			p = next;
		} while (p != *origBox);
	}

	return;
}

void transferParticle(Particle *p, Particle **from, Particle **to)
{
	if (p->prev == p)
		*from = NULL;
	else
	{
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
	
	addToBox(p, to);

}

void addToBox(Particle *p, Particle **box)
{
	if (*box == NULL)
	{
		*box = p;
		p->prev = p;
		p->next = p;
	} else
	{
		p->next = *box;
		p->prev = (*box)->prev;
		p->prev->next = p;
		p->next->prev = p;
		*box = p;
	}
}
