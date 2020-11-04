#include "World.hpp"
#include "ChunkManager.hpp"
#include "Chunk.hpp"


const unsigned int	World::NOISE_STRETCH = 64;

const unsigned int	World::NOISE_SIZE = ChunkManager::SIZES_VOXELS.x / NOISE_STRETCH * ChunkManager::SIZES_VOXELS.z / NOISE_STRETCH;

const unsigned int	World::HEIGHT_MID = ChunkManager::SIZES_VOXELS.y / 2;
const unsigned int	World::HEIGHT_AMPLITUDE = ChunkManager::SIZES_VOXELS.y / 16;

const double		World::BIOME_STEP = 0.25;

const unsigned int	World::CAVERN_COUNT = 4096;
const unsigned int	World::CAVERN_SIZE = 64;
const float			World::CAVERN_STEP = 0.5;

World::World(unsigned int seed):
	_seed(seed),
	_heightmap(NOISE_SIZE),
	_biomes(NOISE_SIZE / 16),
	_caverns(NOISE_SIZE * ChunkManager::SIZES_VOXELS.y / NOISE_STRETCH * 8),
	_holes()
{}

double				World::_setLayers(std::array<unsigned int, 3>& layers_voxel, unsigned int& mid_layer_size, unsigned int& top_layer_size, double amplitude, double biome) const {
	double height = HEIGHT_MID;
	double height_desert = amplitude * HEIGHT_AMPLITUDE / 16;
	double height_mountains = glm::abs((amplitude + 1.0) * HEIGHT_AMPLITUDE * 4);
	double height_hills = amplitude * HEIGHT_AMPLITUDE;
	if (biome < -BIOME_STEP) {
		layers_voxel = {Chunk::Voxel::Rock, Chunk::Voxel::Dirt, Chunk::Voxel::Sand};
		double intensity_biome = -((biome + BIOME_STEP) / (1 - BIOME_STEP));
		height += Noise::interpolateCosine(height_hills, height_desert, -biome);
		mid_layer_size = Noise::interpolateCosine(10, 30, intensity_biome);
		top_layer_size = Noise::interpolateCosine(0, 40, intensity_biome);
	}
	else if (biome > BIOME_STEP) {
		layers_voxel = {Chunk::Voxel::Rock, Chunk::Voxel::Rock, Chunk::Voxel::Grass};
		double intensity_biome = (biome - BIOME_STEP) / (1 - BIOME_STEP);
		height += Noise::interpolateCosine(height_hills, height_mountains, biome);
		if (height > 180) {
			layers_voxel[2] = Chunk::Voxel::Rock;
		}
	}
	else {
		layers_voxel = {Chunk::Voxel::Rock, Chunk::Voxel::Dirt, Chunk::Voxel::Grass};
		if (biome > 0.0) {
			double intensity_biome = biome / BIOME_STEP;
			height += Noise::interpolateCosine(height_hills, height_mountains, biome);
			mid_layer_size = Noise::interpolateCosine(10, 1, intensity_biome);
		}
		else if (biome < 0.0) {
			double intensity_biome = -(biome / BIOME_STEP);
			height += Noise::interpolateCosine(height_hills, height_desert, -biome);
		}
		else {
			height += height_hills;
		}
	}
	return height;
}

bool				World::fillChunk(std::array<std::array<std::array<char, 32>, 32>, 32>& voxels, glm::vec3 pos) const {
	bool	empty = true;

	for (int z = 0; z < Chunk::SIZE; z++) {
		for (int x = 0; x < Chunk::SIZE; x++) {
			std::array<unsigned int, 3> layers_voxel;
			unsigned int mid_layer_size = 10;
			unsigned int top_layer_size = 0;

			double amplitude = _cached_amplitude[x][z];
			double biome = _cached_biome[x][z];

			double height = _setLayers(layers_voxel, mid_layer_size, top_layer_size, amplitude, biome);
			for (int y = 0; y < Chunk::SIZE; y++) {
				double dist = height - (y + pos.y);
				if (dist < -0.5) {
					voxels[x][y][z] = Chunk::Voxel::Empty;
				}
				else if (dist > mid_layer_size + 0.5) {
					voxels[x][y][z] = layers_voxel[0];
					empty = false;
				}
				else if (dist > top_layer_size + 0.5) {
					voxels[x][y][z] = layers_voxel[1];
					empty = false;
				}
				else {
					voxels[x][y][z] = layers_voxel[2];
					empty = false;
				}
				if (y + pos.y > 1 && voxels[x][y][z] != Chunk::Voxel::Empty) {
					double hole_value = _caverns.perlin3d(4, 1.0, 0.5, (x + pos.x) / NOISE_STRETCH * 2, (y + pos.y) / NOISE_STRETCH * 2, (z + pos.z) / NOISE_STRETCH * 2);
					hole_value = glm::abs(hole_value);
					double hole_step = 0.25;
					double sub_height = height - 20;
					double mid_height = height / 2;
					if (y + pos.y > sub_height) {
						hole_step = Noise::interpolateLinear(0.25, 0.6, (y + pos.y - sub_height) / 20);
						hole_step = glm::clamp(hole_step, 0.25, 0.6);
					}
					else if (y + pos.y < mid_height) {
						hole_step = Noise::interpolateLinear(1.0, 0.25, (y + pos.y) / mid_height);
						hole_step = glm::clamp(hole_step, 0.25, 1.0);
					}
					if (hole_value > hole_step) {
						voxels[x][y][z] = Chunk::Voxel::Empty;
					}
				}
			}
		}
	}
	return empty;
}

float				World::heigthAt(unsigned int x, unsigned int z) const {
	double amplitude = _heightmap.perlin2d(4, 1.0, 0.5, x / NOISE_STRETCH, z / NOISE_STRETCH);
	double biome = _biomes.perlin2d(4, 1.0, 0.5, x / NOISE_STRETCH / 4, z / NOISE_STRETCH / 4);
	double height = HEIGHT_MID;
	double height_desert = amplitude * HEIGHT_AMPLITUDE / 16;
	double height_mountains = glm::abs((amplitude + 1.0) * HEIGHT_AMPLITUDE * 4);
	double height_hills = amplitude * HEIGHT_AMPLITUDE;

	if (biome < -BIOME_STEP) {
		height += Noise::interpolateCosine(height_hills, height_desert, -biome);
	}
	else if (biome > BIOME_STEP) {
		height += Noise::interpolateCosine(height_hills, height_mountains, biome);
	}
	else {
		if (biome > 0.0) {
			height += Noise::interpolateCosine(height_hills, height_mountains, biome);
		}
		else if (biome < 0.0) {
			height += Noise::interpolateCosine(height_hills, height_desert, -biome);
		}
		else {
			height += height_hills;
		}
	}
	return height;
}

void			World::cacheAmplitudeAt(float x, float z) {
	for (int i = 0; i < Chunk::SIZE; i++) {
		for (int  j = 0; j < Chunk::SIZE; j++) {
			_cached_amplitude[i][j] = _heightmap.perlin2d(4, 1.0, 0.5,
				(i + Chunk::SIZE * x) / NOISE_STRETCH,
				(j + Chunk::SIZE * z) / NOISE_STRETCH);
		}
	}
}

void			World::cacheBiomeAt(float x, float z) {
	for (int i = 0; i < Chunk::SIZE; i++) {
		for ( int  j = 0; j < Chunk::SIZE; j++) {
			_cached_biome[i][j] = _biomes.perlin2d(4, 1.0, 0.5,
				(i + Chunk::SIZE * x) / NOISE_STRETCH / 4,
				(j + Chunk::SIZE * z) / NOISE_STRETCH / 4);
		}
	}
}

