#include "World.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	World::_SIZES = {
	16384,
	256,
	16384
};

World::World(unsigned int seed): _seed(seed) {
	std::srand(_seed);
}

World::World(void): _seed(time(NULL)) {
	std::srand(_seed);
}