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
	double maxTime;
	bool dumpFillFails;

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
bool allocWorld(void);
void fillWorld(void);
void freeWorld(void);
void dumpWorld(void);
void densityDump(void);
void stepWorld(void);
void sanityCheck(void);

extern World world;
extern Config config;
#ifdef BROWNIAN
extern Particle huge;
#endif

#endif
