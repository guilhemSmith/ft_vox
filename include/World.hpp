#ifndef WORLD_HPP
# define WORLD_HPP

# include "Noise.hpp"
# include "Cavern.hpp"
# include <array>
# include <unordered_map>
# include <glm/glm.hpp>

class World {
private:
	unsigned int const							_seed;
	Noise										_heightmap;
	Noise										_biomes;
	Noise										_caverns;

	std::array<std::array<double, 32>, 32>		_cached_amplitude;
	std::array<std::array<double, 32>, 32>		_cached_biome;
	std::vector<Cavern>							_cached_cavern;
	std::vector<glm::vec3>						_cached_holes_xz;

	double							_setLayers(std::array<unsigned int, 3>& layers_voxel, unsigned int& mid_layer_size, unsigned int& top_layer_size, double amplitude, double biome) const;
public:
	World(unsigned int seed);

	float							heigthAt(unsigned int x, unsigned int z) const;

	bool							fillChunk(std::array<std::array<std::array<char, 32>, 32>, 32>& voxels, glm::vec3 pos) const;
	void							cacheAmplitudeAt(float x, float z);
	void							cacheBiomeAt(float x, float z);
	void							cacheHolesNear(float x, float z);
	void							cacheCavernsAround(float x, float z);

	static const unsigned int		NOISE_STRETCH;
	static const unsigned int		NOISE_SIZE;
	static const unsigned int		HEIGHT_MID;
	static const unsigned int		HEIGHT_AMPLITUDE;
	static const double				BIOME_STEP;
};

#endif