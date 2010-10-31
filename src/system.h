#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define MEM_ERROR(n) (fprintf(stderr, "Not enough memory for %ld bytes\n", (long) n))

#include <stdbool.h>
#include "vmath.h"

typedef struct particle
{
	Vec3 pos;
	Vec3 vel;
	struct particle *prev, *next;
} Particle;

typedef struct box
{
	Particle *p;
	int n;
} Box;

typedef struct world
{
	Box *grid;
	Particle *parts;
} World;

typedef struct config
{
	bool render;
	bool dump;
	bool bench;
	int maxTime;

	/* The same for every dimension */
	float boxSize;
	int numBox;
	int numParticles;
	int iterations;
	float radius;
	float timeStep;
#ifdef BROWNIAN
	float radiusHuge;
	float massHuge;
#endif
} Config;

int main(int argc, char ** argv);
Particle *collides(const Particle *p);
Particle *collideWith(const Particle *p, Particle *ps);
void collideWalls(int ix, int iy, int iz);
void handleCollision(Particle *p1, Particle *p2);
#ifdef BROWNIAN
void handleCollisionHuge(Particle *p);
#endif
Box *boxFromParticle(const Particle *p);
Box *boxFromIndex(int nx, int ny, int nz);
void addToBox(Particle *p, Box *b);
void removeFromBox(Particle *p, Box *from);
bool allocWorld(void);
void freeWorld(void);
void dumpWorld(void);
void densityDump(void);
void stepWorld(void);

extern World world;
extern Config config;
#ifdef BROWNIAN
extern Particle huge;
#endif

#endif
