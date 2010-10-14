#ifndef _WORLD_H_
#define _WORLD_H_

#include <vector>
#include "Box.hpp"

class World
{
	vector<Box> grid;

public:
	World(int nx, int ny, int nz);
private:
	int nx, ny, nz;
	Box &box(int, int, int) const;
};

#endif
