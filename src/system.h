#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#define MEM_ERROR(n) (fprintf(stderr, "Not enough memory for %ld bytes\n", (long) n))

#include <stdbool.h>
#include "vmath.h"

typedef struct particle
{
	Vec3 pos;
	Vec3 vel;
	float r;
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
	/* The same for every dimension */
	float boxSize;
	int numBox;
	int numParticles;
	float radius;
	float timeStep;
} Config;

typedef struct stats
{
	int misses;
	float temperature;
} Stats;

int main(int argc, char ** argv);
Particle *collides(const Particle *p);
Particle *collideWith(const Particle *p, Particle *ps);
void handleCollision(Particle *p1, Particle *p2);
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
extern Stats stats;

#endif
