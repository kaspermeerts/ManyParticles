#include <assert.h>
#include <math.h>
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

Particle *collides(const Particle *p)
{
	Particle *other;
	Box *b;
	int ix, iy, iz;
	int nx, ny, nz;

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
Particle *collideWith(const Particle *p, Particle *ps)
{
	Particle *other = ps;
	Vec3 diff;
	
	if (ps == NULL)
		return NULL;

	do
	{
		if (p == other)
		{
			other = other ->next;
			continue;
		}

		sub(&p->pos, &other->pos, &diff);
		if (length2(&diff) < (p->r + other->r)*(p->r + other->r))
			return other;

		other = other -> next;
	} while (other != ps);

	return NULL;
}

void handleCollision(Particle *p1, Particle *p2)
{
	Vec3 dv, dr, pos1, pos2, dx1, dx2, d;
	Vec3 comv, comv1; /* Center Of Mass velocity */
	Vec3 dv1, dv2; /* Change of velocity after collision */
	float dvt1; 
	/* difference of velocity in the direction of the tangent */
	float dt, dt2, dummy;
	float drsq, dvsq, dvdr, mindist;

	/* First, backtrack the movements of the particle to the moment they
	 * were just touching */
	sub(&p1->pos, &p2->pos, &dr);
	sub(&p1->vel, &p2->vel, &dv);

	drsq = length2(&dr);
	dvsq = length2(&dv); /* dv2 is the square of the velocity difference */
	dvdr = dot(&dv, &dr);
	mindist = p1->r + p2->r;

	dummy = sqrt(dvdr*dvdr - dvsq*(drsq - mindist * mindist));
	dt = (dvdr + dummy ) / dvsq;
	dt2 = (dvdr - dummy ) / dvsq;

	/*printf("%f %f\n", dt, dt2);*/

/*	if (fabs(dt) > fabs(dt2))
		dt = dt2;
*/
	
	scale(&p1->vel, dt, &dx1);
	add(&p1->pos, &dx1, &pos1);
	scale(&p2->vel, dt, &dx2);
	add(&p1->pos, &dx2, &pos2);

	/* TODO Implement mass */
	normalize(&dr, &d);
	add(&p1->vel, &p2->vel, &comv);
	scale(&comv, 0.5, &comv);
	sub(&p1->vel, &comv, &comv1);
	dvt1 = dot(&comv1, &d);
	scale(&d, -2*dvt1, &dv1);
	scale(&d, 2*dvt1, &dv2);

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



void stepWorld(void)
{
	Box *origBox, *newBox;
	float dt = config.timeStep;
	float worldSize = config.numBox * config.boxSize;
	int nb = config.numBox;
	int i, j;

	/* Advance each particle and if necessary, put them in the new box
	 * where they belong */
	for (i = 0; i < nb*nb*nb; i++)
	{
		Particle *p;
		origBox = &world.grid[i];

		p = origBox->p;
		if (p == NULL)
			continue;

		for (j = 0; j < origBox->n; j++)
		{
			/* Since p might be removed, we keep a pointer to 
			 * its successor */
			Particle *next = p->next;
			Vec3 dx;

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
	
	/* Second run, handle all collisions
	 * XXX Does it make sense to do this in one run? */
	for (i = 0; i < nb*nb*nb; i++)
	{
		Box *box = &world.grid[i];
		Particle *p = box->p;

		for (j = 0; j < box->n; j++)
		{
			Particle *p2 = collides(p);

			if (p2) handleCollision(p, p2);
			p = p->next;
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
