#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <list>
#include "math.hpp"

using namespace std;
class Particle
{
public:
	vec3 x;
	float r;

	void interact(Particle &other);
	bool collides(const list<Particle> &parts) const;
	bool collides(const Particle &other) const;
};

#endif
