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
} Stats;

int main(int argc, char ** argv);
bool collides(const Particle *p);
Box *boxFromParticle(const Particle *p);
Box *boxFromIndex(int nx, int ny, int nz);
bool collideWith(const Particle *p, const Particle *ps);
bool fillWorld(void);
void freeWorld(void);
void dumpWorld(void);
void sanityCheck(void);
void densityDump(void);
void stepWorld(void);
void addToBox(Particle *p, Box *b);
void removeFromBox(Particle *p, Box *from);

extern World world;
extern Config config;
extern Stats stats;


#endif
