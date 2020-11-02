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
	_caverns(CAVERN_COUNT * CAVERN_SIZE),
	_holes()
{
	for (int w = 0; w < CAVERN_COUNT; w++) {
		glm::vec3 pos = {
			w % 64 * ChunkManager::SIZES_VOXELS.x / 64,
			0,
			w / 64 * ChunkManager::SIZES_VOXELS.z / 64
		};
		pos.y = heigthAt(pos.x, pos.z) + 10;
		float yaw = 0.0f;
		float pitch = -90.0f;
		for (float s = 0; s < CAVERN_SIZE; s += CAVERN_STEP) {
			double amplitude = _caverns.noise1dSmoothLinear(w * CAVERN_SIZE + s);
			if (amplitude < 0) {
				if (amplitude < -0.5) {
					yaw += Noise::interpolateLinear(0.0, -30.0, (-amplitude - 0.5) / 0.5);
					pitch += Noise::interpolateLinear(30.0, 0.0, (-amplitude - 0.5) / 0.5);
				}
				else {
					yaw += Noise::interpolateLinear(0.0, 30.0, 1 + amplitude / 0.5);
					pitch += Noise::interpolateLinear(30.0, 0.0, 1 + amplitude / 0.5);
				}
			}
			else {
				if (amplitude > 0.5) {
					yaw += Noise::interpolateLinear(0.0, -30.0, (amplitude - 0.5) / 0.5);
					pitch += Noise::interpolateLinear(-30.0, 0.0, (amplitude - 0.5) / 0.5);
				}
				else {
					yaw += Noise::interpolateLinear(0.0, 30.0, 1 - amplitude / 0.5);
					pitch += Noise::interpolateLinear(-30.0, 0.0, 1 - amplitude / 0.5);
				}
			}
			pitch = glm::clamp(pitch, -10.0f, -170.0f);
			glm::vec3 dir = {
				glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
				glm::sin(glm::radians(pitch)),
				glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
			};
			pos = pos + glm::normalize(dir) * 4.0f;
			_holes.emplace_back(pos);
		}
	}
}

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
	std::vector<glm::vec3>	close_holes = std::vector<glm::vec3>();

	for (auto &hole : _holes) {
		if ((hole.x + 5 >= pos.x && hole.x - 5 < pos.x + Chunk::SIZE)
			&& (hole.y + 5 >= pos.y && hole.y - 5 < pos.y + Chunk::SIZE)
			&& (hole.z + 5 >= pos.z && hole.z - 5 < pos.z + Chunk::SIZE)) {
			close_holes.emplace_back(hole);
		}
	}
	for (int z = 0; z < Chunk::SIZE; z++) {
		for (int x = 0; x < Chunk::SIZE; x++) {
			std::array<unsigned int, 3> layers_voxel;
			unsigned int mid_layer_size = 10;
			unsigned int top_layer_size = 0;

			double amplitude = _heightmap.perlin2d(4, 1.0, 0.5, (x + pos.x) / NOISE_STRETCH, (z + pos.z) / NOISE_STRETCH);
			double biome = _biomes.perlin2d(4, 1.0, 0.5, (x + pos.x) / NOISE_STRETCH / 4, (z + pos.z) / NOISE_STRETCH / 4);

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
				if (voxels[x][y][z] != Chunk::Voxel::Empty) {
					glm::vec3 pos_voxel(pos.x + x, pos.y + y, pos.z + z);
					for (auto &hole : close_holes) {
						if (glm::distance(hole, pos_voxel) < 5) {
							voxels[x][y][z] = Chunk::Voxel::Empty;
							break;
						}
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