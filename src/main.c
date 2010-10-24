#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include "vmath.h"
#include "system.h"
#include "render.h"
#include "setup.h"

void sanityCheck(void);
int renderLoop(void);

int renderLoop(void)
{
	SDL_Event event;

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return 0;
					break;
				default:
					break;
				}
			}
		}

		render();
		stepWorld();
	}
}

void sanityCheck()
{
	int i, j, nParts1, nParts2;
	const Particle *p1;
	float totKE = 0;
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

	printf("%f\n", totKE);

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

int main(int argc, char **argv)
{
	bool rendering = false;
	int iterations, i;

	if (argc < 6)
	{
		fprintf(stderr, "This needs 5 arguments\n");
		return 1;
	}

	config.boxSize = atof(argv[1]);
	config.numBox = atoi(argv[2]);
	config.numParticles = atoi(argv[3]);
	config.radius = atof(argv[4]);
	iterations = atoi(argv[5]);
	config.timeStep = 0.01;

	stats.misses = 0;

	srand(time(NULL));

	allocWorld();
	fillWorld();
	
	if (rendering)
	{
		initRender();
		renderLoop();
	}
	else
		for (i = 0; i < iterations; i++)
			stepWorld();

	sanityCheck();
	freeWorld();

	return 0;
}
