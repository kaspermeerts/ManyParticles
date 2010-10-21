#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include "vmath.h"
#include "system.h"
#include "render.h"

int main(int argc, char **argv)
{
	SDL_Event event;

	if (argc < 5)
	{
		fprintf(stderr, "This needs 5 arguments\n");
		return 1;
	}

	config.boxSize = atof(argv[1]);
	config.numBox = atoi(argv[2]);
	config.numParticles = atoi(argv[3]);
	config.radius = atof(argv[4]);
	config.timeStep = 0.1;

	stats.misses = 0;

	srand(1);

	fillWorld();

	/*dumpWorld();*/

	/*initRender();*/

	while (0)
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
		/*stepWorld();*/
	}

	stepWorld();

	/*densityDump();*/

	sanityCheck();

	freeWorld();

	return 0;
}
