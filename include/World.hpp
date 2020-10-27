#ifndef WORLD_HPP
# define WORLD_HPP

# include "Noise.hpp"
# include <glm/glm.hpp>

class World {
private:
	unsigned int const					_seed;
	Noise								_heightmap;
	Noise								_biomes;
	Noise								_caverns;
public:
	World(unsigned int seed);

	bool								fillChunk(std::array<std::array<std::array<char, 32>, 32>, 32>& voxels, glm::vec3 pos) const;

	static const unsigned int			NOISE_STRETCH;
	static const unsigned int			NOISE_SIZE;
};

#endif