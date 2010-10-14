#include "Particle.hpp"
#include "math.hpp"

bool Particle::collides(const list<Particle> &parts) const
{
	for (list<Particle>::const_iterator other = parts.begin(); 
			other != parts.end();
			other++)
	{
		if (collides(*other))
			return true;
	}

	return false;
}

bool Particle::collides(const Particle &other) const
{
	vec3 diff = x - other.x;
	return diff.length() > r + other.r;
}


