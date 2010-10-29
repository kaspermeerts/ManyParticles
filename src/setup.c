#include <stdlib.h>
#include "system.h"
#include "setup.h"

void fillWorld(void)
{
	Box *box;
	int i;
	Particle *ps = world.parts;
	float v, u, phi;
	float worldSize = config.numBox * config.boxSize;

#ifdef BROWNIAN
	huge.pos.x = worldSize / 2;
	huge.pos.y = worldSize / 2;
	huge.pos.z = worldSize / 2;

	huge.vel.x = 0;
	huge.vel.y = 0;
	huge.vel.z = 0;

	config.radiusHuge = config.radius * 20;
	config.massHuge = 1;
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
		} while (collides(&ps[i]));

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
	
	/*
	ps[0].pos.x = 0.2;
	ps[0].pos.y = 0.5;
	ps[0].pos.z = 0.5;

	ps[0].vel.x = 0.20;
	ps[0].vel.y = 0.05;
	ps[0].vel.z = 0.0;

	ps[1].pos.x = 0.8;
	ps[1].pos.y = 0.5;
	ps[1].pos.z = 0.5;

	ps[1].vel.x = 0.00;
	ps[1].vel.y = 0.0;
	ps[1].vel.z = 0.0;
	*/
}	
