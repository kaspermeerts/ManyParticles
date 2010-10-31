#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "main.h"
#include "vmath.h"
#include "system.h"
#include "render.h"
#include "setup.h"

void sanityCheck(void);
int renderLoop(void);
void parseArguments(int argc, char **argv);
void printStats(void);

static int renderLoop(void)
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

static void printStats()
{
	int i;
	const Particle *p;
	float totKE = 0;
	float v2;
	Vec3 totP = {0, 0, 0};

	for (i = 0; i < config.numParticles; i++)
	{
		p = &world.parts[i];
		v2 = length2(&p->vel);
		totKE += v2/2;
		add(&totP, &p->vel, &totP);
	}

	printf("P: ");
	printVector(&totP);
	printf(" E: %f\n", totKE);

	return;
}

static void sanityCheck()
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

static void parseArguments(int argc, char **argv)
{
	int c;

	/* Sane defaults */
	config.iterations = 100;
	config.timeStep = 0.01;
	config.maxTime = -1;


	while ((c = getopt(argc, argv, ":i:t:rdb:")) != -1)
	{
		switch (c)
		{
		case 'i':
			config.iterations = atoi(optarg);
			if (config.iterations < 0)
				die("Invalid number of iterations %d\n",
						config.iterations);
			break;
		case 't':
			config.timeStep = atof(optarg);
			if (config.timeStep <= 0)
				die("Invalid timestep %f\n", config.timeStep);
			break;
		case 'r':
			config.render = true;
			break;
		case 'd':
			config.dump = true;
			break;
		case 'b':
			config.bench = true;
			if (optarg != NULL)
				config.maxTime = atof(optarg) / 1000;
			break;
		case ':':
			die("Option -%c requires an argument\n", optopt);
			break;
		case '?':
			die("Option -%c not recognized\n", optopt);
			break;
		default:
			/* XXX */
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc < 4)
		die("Usage: main <box size> <box number> <particle number> "
					"<radius>\n"
		    "            [-t timestep] [-i iterations] [-b [max milisec]]\n");

	config.boxSize = atof(argv[0]);
	config.numBox = atoi(argv[1]);
	config.numParticles = atoi(argv[2]);
	config.radius = atof(argv[3]);

	return;
}

void die(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	exit(1);
	return;
}

static double timeDifference(struct rusage *begin, struct rusage *end)
{
	struct timeval *b = &begin->ru_utime;
	struct timeval *e = &end->ru_utime;

	return e->tv_sec - b->tv_sec  +  
			(double)(e->tv_usec - b->tv_usec)/1000000;
}


int main(int argc, char **argv)
{
	int i;
	struct rusage startTime, endTime;
	double timeDiff;


	parseArguments(argc, argv);

	srand(time(NULL));

	allocWorld();
	fillWorld();
	
	if (config.render)
	{
		initRender();
		renderLoop();
	}
	else
	{
		if (config.bench)
			getrusage(RUSAGE_SELF, &startTime);

		for (i = 0; i < config.iterations; i++)
		{
#ifdef BROWNIAN
			if (i%10 == 0)
			{
				printVector(&huge.pos);
				printf("\n");
			}
#endif
			stepWorld();

			if (config.bench && config.maxTime > 0)
			{
				getrusage(RUSAGE_SELF, &endTime);
				timeDiff = timeDifference(&startTime, &endTime);
				if (timeDiff > config.maxTime)
				{
					i++;
					break;
				}
			}
		}

		if (config.bench)
		{
			getrusage(RUSAGE_SELF, &endTime);
			timeDiff = timeDifference(&startTime, &endTime);
			printf("%f\n", i / timeDiff);
		}
	}

	if (!config.bench)
		sanityCheck();

#ifndef BROWNIAN
	if (config.dump)
		dumpWorld();
#endif
	freeWorld();

	return 0;
}
