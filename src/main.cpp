#include <iostream>
#include "Particle.hpp"

using namespace std;

int main(void)
{
	vec3 a = vec3(1,2,3);
	vec3 b = vec3(5,6,7);
	vec3 c = a + b;

	cout << c.x << endl;

	return 0;
}
