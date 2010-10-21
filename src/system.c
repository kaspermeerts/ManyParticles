#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vmath.h"
#include "system.h"

World world;
Config config;
Stats stats;


Box *boxFromParticle(const Particle *p)
{
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	return boxFromIndex(nx, ny, nz);
}

Box *boxFromIndex(int ix, int iy, int iz)
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
	Box *b;
	int ix, iy, iz;
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	b = boxFromIndex(nx, ny, nz);
	if (collideWith(p, b->p))
		return true;

	for (ix = -1; ix <= 1; ix++)
	for (iy = -1; iy <= 1; iy++)
	for (iz = -1; iz <= 1; iz++)
	{
		if (ix == 0 && iy == 0 && iz == 0)
			continue;

		b = boxFromIndex(nx+ix, ny+iy, nz+iz);
		if (collideWith(p, b->p))
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
	Box *box;
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
	Box *box = boxFromIndex(n, n, n);
	Particle *p = box->p;

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
	int i, j, nParts1, nParts2;
	const Particle *p1, *p2;
	nParts1 = 0;
	nParts2 = 0;

	for (i = 0; i < config.numParticles; i++)
	{
		p1 = &world.parts[i];
		/*
		for (j = i + 1; j < config.numParticles; j++)
		{
			float d;
			p2 = &world.parts[j];
			d = distance(&p1->pos, &p2->pos);
			if (d < p1->r + p2->r)
				fprintf(stderr, "%f %f %f PROBLEM?\n", 
						d, p1->r, p2->r);
		}
		*/
		if (p1->next->prev != p1 || p1->prev->next != p1)
			fprintf(stderr, "%p is in a borked list\n", p1);
	}

	for (i = 0; i < config.numBox * config.numBox * config.numBox; i++)
	{
		Box *b = &world.grid[i];
		const Particle *p, *first;

		if (b->p == NULL)
		{
			if (b->n != 0)
				fprintf(stderr, "Box %d: found zero, expected"
						" %d\n", i, b->n);
			continue;
		}

		first = b->p;
		p = first;
		j = 0;
		do
		{
			Box *correctBox = boxFromParticle(p);
			if (correctBox != b)
			{
				fprintf(stderr, "Particle is in box %d, should be in %d\n", i, (correctBox - world.grid)/sizeof(*correctBox));
			}
			j++;
			nParts1++;
			p = p->next;
		} while (p != first);

		if (j != b->n)
		{
			fprintf(stderr, "Box %d: found %d, expected %d\n", 
					i, j, b->n);
		}
		nParts2 += b->n;
	}

	if (nParts1 != config.numParticles)
	{
		fprintf(stderr, "1: Found a total of %d particles, should be %d\n",
				nParts1, config.numParticles);
	}

	if (nParts2 != config.numParticles)
	{
		fprintf(stderr, "2: Found a total of %d particles, should be %d\n",
				nParts2, config.numParticles);
	}
	return;
}

void stepWorld(void)
{
	Particle *p, *next;
	Box *origBox, *newBox;
	float dt = config.timeStep;
	float worldSize = config.numBox * config.boxSize;
	int nb = config.numBox;
	Vec3 dx;
	int i, j;

	for (i = 0; i < nb*nb*nb; i++)
	{
		origBox = &world.grid[i];

		p = origBox->p;
		for (j = 0; j < origBox->n; j++)
		{
			assert(p != NULL);
			next = p->next;

			scale(&p->vel, dt, &dx);
			add(&p->pos, &dx, &p->pos);

			if (p->pos.x >= worldSize)
				p->pos.x -= worldSize;
			else if (p->pos.x < 0)
				p->pos.x += worldSize;
			
			if (p->pos.y >= worldSize)
				p->pos.y -= worldSize;
			else if (p->pos.y < 0)
				p->pos.y += worldSize;

			if (p->pos.z >= worldSize)
				p->pos.z -= worldSize;
			else if (p->pos.z < 0)
				p->pos.z += worldSize;


			newBox = boxFromParticle(p);
			
			if (newBox != origBox)
			{
				removeFromBox(p, origBox);
				addToBox(p, newBox);
			}
			
			p = next;
		}
	}

	return;
}

void removeFromBox(Particle *p, Box *from)
{
	assert(p != NULL);
	assert(from->n != 0);

	if (from->n == 1)
	{
		assert(p->prev == p);
		assert(p->next == p);
		from->p = NULL;
	}
	else
	{
		assert(p->prev->next == p);
		assert(p->next->prev == p);
		p->prev->next = p->next;
		p->next->prev = p->prev;

		if (from->p == p)
			from->p = p->next;

	}

	p->prev = NULL;
	p->next = NULL;

	from->n--;

}

void addToBox(Particle *p, Box *b)
{
	if (b->p == NULL)
	{
		assert(b->n == 0);
		b->p = p;
		p->prev = p;
		p->next = p;
	} else
	{
		assert(b->n > 0);
		p->next = b->p;
		p->prev = b->p->prev;
		p->prev->next = p;
		p->next->prev = p;
	}

	b->n++;
}
