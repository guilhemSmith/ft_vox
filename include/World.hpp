#ifndef WORLD_HPP
# define WORLD_HPP

# include <glm/glm.hpp>

class World {
private:
	unsigned int const	_seed;

	static const glm::u32vec3	_SIZES;

public:
	World(void);
	World(unsigned int seed);
};

#endif