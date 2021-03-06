/*
 * Many-particle simulation software
 * Copyright (C) 2010 Roald Frederickx and Kasper Meerts
 *
 * This program is released under the terms of the license contained
 * in the file LICENSE.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vmath.h"
#include "system.h"

World world;
Config config;
#ifdef BROWNIAN
Particle huge;
#endif

static Particle *collides(const Particle *p);
static Particle *collideWith(const Particle *p, Particle *ps);
static void collideWalls(int ix, int iy, int iz);
static void handleCollision(Particle *p1, Particle *p2);
#ifdef BROWNIAN
static void handleCollisionHuge(Particle *p);
#endif
static Box *boxFromParticle(const Particle *p);
static Box *boxFromIndex(int nx, int ny, int nz);
static void addToBox(Particle *p, Box *b);
static void removeFromBox(Particle *p, Box *from);


static Box *boxFromParticle(const Particle *p)
{
	int nx, ny, nz;

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	return boxFromIndex(nx, ny, nz);
}

static Box *boxFromIndex(int ix, int iy, int iz)
{
	int nb = config.numBox;
	
	if (ix < 0)
		ix = 0;
	else if (ix >= nb)
		ix = nb - 1;

	if (iy < 0)
		iy = 0;
	else if (iy >= nb)
		iy = nb - 1;

	if (iz < 0)
		iz = 0;
	else if (iz >= nb)
		iz = nb - 1;

	return world.grid + ix*nb*nb + iy*nb + iz;
}

static Particle *collides(const Particle *p)
{
	Particle *other;
	Box *b;
	int ix, iy, iz;
	int nx, ny, nz;

#ifdef BROWNIAN
	const float d = config.radius + config.radiusHuge;
	Vec3 dhuge;
	sub(&p->pos, &huge.pos, &dhuge);
	if (length2(&dhuge) < d*d)
		return &huge;
#endif

	nx = p->pos.x / config.boxSize;
	ny = p->pos.y / config.boxSize;
	nz = p->pos.z / config.boxSize;

	b = boxFromIndex(nx, ny, nz);
	other = collideWith(p, b->p);
	if (other != NULL)
		return other;

	for (ix = -1; ix <= 1; ix++)
	for (iy = -1; iy <= 1; iy++)
	for (iz = -1; iz <= 1; iz++)
	{
		if (ix == 0 && iy == 0 && iz == 0)
			continue;

		b = boxFromIndex(nx+ix, ny+iy, nz+iz);
		other = collideWith(p, b->p);
		if (other != NULL)
			return other;
	}

	return NULL;
}

/* p is the particle we're checking, ps is the first particle in the box */
static Particle *collideWith(const Particle *p, Particle *ps)
{
	Particle *other = ps;
	Vec3 diff;
	const float r = config.radius;
	
	if (ps == NULL)
		return NULL;

	do
	{
		if (p == other)
		{
			other = other->next;
			continue;
		}

		sub(&p->pos, &other->pos, &diff);
		if (length2(&diff) < 4*r*r)
			return other;

		other = other->next;
	} while (other != ps);

	return NULL;
}

#ifdef BROWNIAN
static void handleCollisionHuge(Particle *p)
{
	Vec3 phuge;
	Vec3 dv, dr, dx1, dx2, d;
	Vec3 comv, comv1; /* Center Of Mass velocity */
	Vec3 dv1, dv2; /* Change of velocity after collision */
	float dvt1; 
	/* Difference of velocity in the direction of the tangent */
	float dt;
	float drsq, dvsq, dvdr, mindist;

	/* First, backtrack the movements of the particle to the moment they
	 * were just touching */
	sub(&p->pos, &huge.pos, &dr);
	sub(&p->vel, &huge.vel, &dv);

	drsq = length2(&dr);
	dvsq = length2(&dv); /* Square of the velocity difference */
	dvdr = dot(&dv, &dr);
	mindist = config.radius + config.radiusHuge;

	dt = (dvdr + sqrt(dvdr*dvdr + dvsq*(mindist * mindist - drsq))) / dvsq;
	if (fabs(dt) > 2 * config.timeStep)
		return;

	scale(&p->vel, -dt, &dx1);
	scale(&huge.vel, -dt, &dx2);

	add(&p->pos, &dx1, &p->pos);
	add(&huge.pos, &dx2, &huge.pos);

	normalize(&dr, &d);
	scale(&huge.vel, config.massHuge, &phuge);
	add(&p->vel, &phuge, &comv);
	scale(&comv, 1.0/(1 + config.massHuge), &comv);
	sub(&p->vel, &comv, &comv1);
	dvt1 = dot(&comv1, &d);
	scale(&d, -2*dvt1, &dv1);
	scale(&d,  2*dvt1/config.massHuge, &dv2);

	scale(&dv1, dt, &dx1);
	scale(&dv2, dt, &dx2);

	add(&p->pos, &dx1, &p->pos);
	add(&p->vel, &dv1, &p->vel);

	add(&huge.pos, &dx2, &huge.pos);
	add(&huge.vel, &dv2, &huge.vel);

	return;
}
#endif

static void handleCollision(Particle *__restrict__ p1, Particle *__restrict__ p2)
{
	Vec3 dv, dr, dx1, dx2, d;
	Vec3 comv, comv1; /* Center Of Mass velocity */
	Vec3 dv1, dv2; /* Change of velocity after collision */
	float dvt1; 
	/* Difference of velocity in the direction of the tangent */
	float dt;
	float drsq, dvsq, dvdr, mindist;
	const float r = config.radius;

	/* First, backtrack the movements of the particle to the moment they
	 * were just touching */
	sub(&p1->pos, &p2->pos, &dr);
	sub(&p1->vel, &p2->vel, &dv);

	drsq = length2(&dr);
	dvsq = length2(&dv); /* Square of the velocity difference */
	dvdr = dot(&dv, &dr);
	mindist = 2*r;

	dt = (dvdr + sqrt(dvdr*dvdr + dvsq*(mindist * mindist - drsq))) / dvsq;

	scale(&p1->vel, -dt, &dx1);
	scale(&p2->vel, -dt, &dx2);

	add(&p1->pos, &dx1, &p1->pos);
	add(&p2->pos, &dx2, &p2->pos);

	normalize(&dr, &d);
	add(&p1->vel, &p2->vel, &comv);
	scale(&comv, 0.5, &comv);
	sub(&p1->vel, &comv, &comv1);
	dvt1 = dot(&comv1, &d);
	scale(&d, -2*dvt1, &dv1);
	scale(&d,  2*dvt1, &dv2);

	scale(&dv1, dt, &dx1);
	scale(&dv2, dt, &dx2);

	add(&p1->pos, &dx1, &p1->pos);
	add(&p1->vel, &dv1, &p1->vel);

	add(&p2->pos, &dx2, &p2->pos);
	add(&p2->vel, &dv2, &p2->vel);

	return;
}

/* Precondition: config MUST be valid
 * Allocates */
bool allocWorld()
{
	int nb = config.numBox;

	world.parts = calloc(config.numParticles, sizeof(Particle));
	if (world.parts == NULL)
	{
		MEM_ERROR(config.numParticles * sizeof(Particle));
		return false;
	}

	world.grid = calloc(nb * nb * nb, sizeof(*(world.grid)));
	if (world.grid == NULL)
	{
		MEM_ERROR(nb * nb * nb * sizeof(*world.grid));
		return false;
	}
	return true;
}

void fillWorld()
{
	Box *box;
	int i;
	Particle *ps = world.parts;
	float v, u, phi;
	float worldSize = config.numBox * config.boxSize;
	long fillFails = 0;

#ifdef BROWNIAN
	huge.pos.x = worldSize / 2;
	huge.pos.y = worldSize / 2;
	huge.pos.z = worldSize / 2;

	huge.vel.x = 0;
	huge.vel.y = 0;
	huge.vel.z = 0;

	config.radiusHuge = config.radius * 20;
	config.massHuge = 10;
#endif

	for (i = 0; i < config.numParticles; i++)
	{
		do
		{
			/* To avoid hitting the exact boundary of the world
			 * shift 32 bits to 23 bits, the precision of a float
			 * mantissa */
			ps[i].pos.x = (rand() >> 9) * worldSize / 
					((RAND_MAX >> 9) + 1U);
			ps[i].pos.y = (rand() >> 9) * worldSize /
					((RAND_MAX >> 9) + 1U);
			ps[i].pos.z = (rand() >> 9) * worldSize / 
					((RAND_MAX >> 9) + 1U);
			fillFails++;
		} while (collides(&ps[i]));
		fillFails--;

		/* Sphere surface point picking is trickier than might seem */
		phi = 2 * M_PI * rand() / (double) (RAND_MAX + 1U);
		u = rand() / (RAND_MAX / 2.) - 1;
		v = 1; /*rand() / (RAND_MAX / 1.);*/

		ps[i].vel.x = v * sqrt(1-u*u) * cos(phi);
		ps[i].vel.y = v * sqrt(1-u*u) * sin(phi);
		ps[i].vel.z = v * u;

		box = boxFromParticle(&ps[i]);
		addToBox(&ps[i], box);
	}
	
	if (config.dumpFillFails)
		printf("%ld\n", fillFails);
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
		printf("%d\t", i);
		printVector(&p->pos);
		printVector(&p->vel);
		printf("\n");
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

static void collideWalls(int ix, int iy, int iz)
{
	int i;
	int nb = config.numBox;
	Particle *p;
	Box *b = boxFromIndex(ix, iy, iz);
	float worldSize = config.boxSize * config.numBox;

	p = b->p;
	for (i = 0; i < b->n; i++)
	{
		if (ix == 0 && p->pos.x < 0)
		{
			p->vel.x = -p->vel.x;
			p->pos.x = -p->pos.x;
		} else if (ix == nb && p->pos.x > worldSize)
		{
			p->vel.x = -p->vel.x;
			p->pos.x = worldSize - (p->pos.x - worldSize);
		}

		if (iy == 0 && p->pos.y < 0)
		{
			p->vel.y = -p->vel.y;
			p->pos.y = -p->pos.y;
		} else if (iy == nb && p->pos.y > worldSize)
		{
			p->vel.y = -p->vel.y;
			p->pos.y = worldSize - (p->pos.y - worldSize);
		}

		if (iz == 0 && p->pos.z < 0)
		{
			p->vel.z = -p->vel.z;
			p->pos.z = -p->pos.z;
		} else if (iz == nb && p->pos.z > worldSize)
		{
			p->vel.z = -p->vel.z;
			p->pos.z = worldSize - (p->pos.z - worldSize);
		}
	
		p = p->next;
	}
}

void stepWorld(void)
{
	Box *origBox, *newBox;
	float dt = config.timeStep;
	int nb = config.numBox;
	int i, j, n;

	/* XXX Does it make sense to do this in one run? */
	for (i = 0; i < nb*nb*nb; i++)
	{
		Box *box = &world.grid[i];
		Particle *p = box->p;

		for (j = 0; j < box->n; j++)
		{
			Particle *p2 = collides(p);
#ifdef BROWNIAN
			if (p2 == &huge)
				handleCollisionHuge(p);
			else
#endif
			if (p2 != NULL)
				handleCollision(p, p2);
			p = p->next;
		}

	}

	for (i = 0; i < nb; i++)
	for (j = 0; j < nb; j++)
	{
		/* TODO less naive */
		collideWalls( i,  j,  0);
		collideWalls( i,  0,  j);
		collideWalls( 0,  i,  j);

		collideWalls( i,  j, nb);
		collideWalls( i, nb,  j);
		collideWalls(nb,  i,  j);
	}
	/* Advance each particle and if necessary, put them in the new box
	 * where they belong */
	for (i = 0; i < nb*nb*nb; i++)
	{
		Particle *p;
		origBox = &world.grid[i];

		p = origBox->p;
		if (p == NULL)
			continue;

		n = origBox->n; /* Save original number because this can 
				   change if we swap out particles in the 
				   loop below! */
		for (j = 0; j < n; j++)
		{
			/* Since p might be removed, we keep a pointer to 
			 * its successor */
			Particle *next = p->next;
			Vec3 dx;

			scale(&p->vel, dt, &dx);
			add(&p->pos, &dx, &p->pos);

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

static void removeFromBox(Particle *p, Box *from)
{
	assert(p != NULL);
	assert(from->n != 0);

	if (from->n == 1)
	{
		assert(p->prev == p);
		assert(p->next == p);
		from->p = NULL;
	} else
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

static void addToBox(Particle *p, Box *b)
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

void sanityCheck()
{
	int i, j, nParts1, nParts2;
	const Particle *p1;
	float totKE = 0;
	Vec3 totP = {0, 0, 0};
	nParts1 = 0;
	nParts2 = 0;

	/* Make sure particle are not too close together and that their linked
	 * list is consistent */
	for (i = 0; i < config.numParticles; i++)
	{
		float v2;
		p1 = &world.parts[i];
		v2 = dot(&p1->vel, &p1->vel);
		totKE += v2/2;
		add(&totP, &p1->vel, &totP);
		/*
		for (j = i + 1; j < config.numParticles; j++)
		{
			const Particle *p2;
			float d;
			p2 = &world.parts[j];
			d = distance(&p1->pos, &p2->pos);
			if (d < p1->r + p2->r)
				fprintf(stderr, "%f %f %f PROBLEM?\n", 
						d, p1->r, p2->r);
		}
		*/
		if (p1->next->prev != p1 || p1->prev->next != p1)
			fprintf(stderr, "%p is in a borked list\n",
					(const void *) p1);
	}

	/* Check if each particle is in the box it should be in given it's
	 * coordinates and count the number of particles and check them with
	 * the total */
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
				fprintf(stderr, "Particle is in box %d, "
				"should be in %ld\n", i, (correctBox -
				world.grid)/sizeof(*correctBox));
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
		fprintf(stderr, "1: Found a total of %d particles, "
			"should be %d\n", nParts1, config.numParticles);
	}

	if (nParts2 != config.numParticles)
	{
		fprintf(stderr, "2: Found a total of %d particles, "
			"should be %d\n", nParts2, config.numParticles);
	}
	return;
}
