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

static int renderLoop(void);
static void parseArguments(int argc, char **argv);
static void printStats(void);

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

static void parseArguments(int argc, char **argv)
{
	int c;

	/* Sane defaults */
	config.iterations = 100;
	config.timeStep = 0.01;
	config.maxTime = -1;
	config.dumpFillFails = false;


	while ((c = getopt(argc, argv, ":i:t:rdb:f")) != -1)
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
		case 'f':
			config.dumpFillFails = true;
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
					"<radius> [flags]\n"
		    "Flags:\n" 
		    " -t <num>  Use timesteps of <num>\n"
		    " -i <num>  Iterate for <num> iterations>\n"
		    " -b <num>  Benchmark for at least <num> miliseconds\n"
		    " -f        Dump amount of failed fills\n"
		    " -r        Render\n");

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
	struct timeval tv;


	parseArguments(argc, argv);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec ^ tv.tv_usec);

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
