#ifndef _BOX_H_
#define _BOX_H_

#include <list>
#include "Particle.hpp"

using namespace std;

class Box
{
	Box *neighs[26];
	list<Particle> particles;

	bool insert(Particle &other);
};

#endif
