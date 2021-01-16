#include "World.hpp"
#include "ChunkManager.hpp"
#include "Chunk.hpp"

const unsigned int	World::NOISE_STRETCH = 64;

const unsigned int	World::NOISE_SIZE = ChunkManager::SIZES_VOXELS.x / NOISE_STRETCH * ChunkManager::SIZES_VOXELS.z / NOISE_STRETCH;

const unsigned int	World::HEIGHT_MID = ChunkManager::SIZES_VOXELS.y / 2;
const unsigned int	World::HEIGHT_AMPLITUDE = ChunkManager::SIZES_VOXELS.y / 16;

const double		World::BIOME_STEP = 0.25;

World::World(unsigned int seed):
	_seed(seed),
	_heightmap(NOISE_SIZE),
	_biomes(NOISE_SIZE / 16),
	_caverns(NOISE_SIZE * ChunkManager::SIZES_VOXELS.y / NOISE_STRETCH * 8),
	_cached_amplitude(),
	_cached_biome(),
	_cached_cavern(),
	_cached_holes_xz()
{}

double				World::_setLayers(std::array<unsigned int, 3>& layers_voxel, unsigned int& mid_layer_size, unsigned int& top_layer_size, double amplitude, double biome) const {
	double height = HEIGHT_MID;
	double height_desert = amplitude * HEIGHT_AMPLITUDE / 16;
	double height_mountains = glm::abs((amplitude + 1.0) * HEIGHT_AMPLITUDE * 4);
	double height_hills = amplitude * HEIGHT_AMPLITUDE;
	if (biome < -BIOME_STEP) {
		layers_voxel = {Chunk::Voxel::Stone, Chunk::Voxel::Dirt, Chunk::Voxel::Sand};
		double intensity_biome = -((biome + BIOME_STEP) / (1 - BIOME_STEP));
		height += Noise::interpolateCosine(height_hills, height_desert, -biome);
		mid_layer_size = Noise::interpolateCosine(10, 30, intensity_biome);
		top_layer_size = Noise::interpolateCosine(0, 40, intensity_biome);
	}
	else if (biome > BIOME_STEP) {
		layers_voxel = {Chunk::Voxel::Stone, Chunk::Voxel::Stone, Chunk::Voxel::Grass};
		double intensity_biome = (biome - BIOME_STEP) / (1 - BIOME_STEP);
		height += Noise::interpolateCosine(height_hills, height_mountains, biome);
		if (height > 180) {
			layers_voxel[2] = Chunk::Voxel::Snow;
		}
	}
	else {
		layers_voxel = {Chunk::Voxel::Stone, Chunk::Voxel::Dirt, Chunk::Voxel::Grass};
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
	std::vector<glm::vec3> holes = std::vector<glm::vec3>();
	
	for (int z = 0; z < Chunk::SIZE; z++) {
		for (int x = 0; x < Chunk::SIZE; x++) {
			std::array<unsigned int, 3> layers_voxel;
			unsigned int mid_layer_size = 10;
			unsigned int top_layer_size = 0;

			double amplitude = _cached_amplitude[x][z];
			double biome = _cached_biome[x][z];

			double height = _setLayers(layers_voxel, mid_layer_size, top_layer_size, amplitude, biome);
			for (int y = Chunk::SIZE - 1; y >= 0; y--) {
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
			}
		}
	}

	for (auto &pos_hole : _cached_holes_xz) {
		if (pos_hole.y + Cavern::HOLE_SIZE >= pos.y && pos_hole.y - Cavern::HOLE_SIZE <= pos.y + Chunk::SIZE) {
			for (int x = pos_hole.x - Cavern::HOLE_SIZE; x <= pos_hole.x + Cavern::HOLE_SIZE; x++) {
				glm::vec3 voxel_pos = glm::ivec3();
				int chunk_x = x - pos.x;
				if (chunk_x < 0 || chunk_x >= Chunk::SIZE) {
					continue;
				}
				voxel_pos.x = chunk_x;
				for (int z = pos_hole.z - Cavern::HOLE_SIZE; z <= pos_hole.z + Cavern::HOLE_SIZE; z++) {
					int chunk_z = z - pos.z;
					if (chunk_z < 0 || chunk_z >= Chunk::SIZE) {
						continue;
					}
					voxel_pos.z = chunk_z;
					for (int y = pos_hole.y - Cavern::HOLE_SIZE; y <= pos_hole.y + Cavern::HOLE_SIZE; y++) {
						int chunk_y = y - pos.y;
						if (chunk_y < 0 || chunk_y >= Chunk::SIZE) {
							continue;
						}
						voxel_pos.y = chunk_y;
						if (glm::distance(voxel_pos + pos, pos_hole) < Cavern::HOLE_SIZE) {
							voxels[voxel_pos.x][voxel_pos.y][voxel_pos.z] = Chunk::Voxel::Empty;
							if (voxel_pos.y > 0 && voxels[voxel_pos.x][voxel_pos.y - 1][voxel_pos.z] == Chunk::Voxel::Dirt) {
								voxels[voxel_pos.x][voxel_pos.y - 1][voxel_pos.z] = Chunk::Voxel::Grass;
							}
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

void			World::cacheHolesNear(float x, float z) {
	_cached_holes_xz.clear();
	for (auto &cave : _cached_cavern) {
		glm::u32vec2	pos = cave.chunkPos();
		if (pos.x >= x - Cavern::DISTANCE_CHUNK_MAX && pos.x <= x + Cavern::DISTANCE_CHUNK_MAX
			&& pos.y >= z - Cavern::DISTANCE_CHUNK_MAX && pos.y <= z + Cavern::DISTANCE_CHUNK_MAX) {
			cave.selectHoles(x, z, _cached_holes_xz);
		}
	}
}

void			World::cacheCavernsAround(float x, float z) {
	std::vector<Cavern>		new_cache = std::vector<Cavern>();
	
	for (int i = x - ChunkManager::LOAD_DISTANCE - Cavern::DISTANCE_CHUNK_MAX; i <= x + ChunkManager::LOAD_DISTANCE + Cavern::DISTANCE_CHUNK_MAX; i++) {
		for (int j = z - ChunkManager::LOAD_DISTANCE - Cavern::DISTANCE_CHUNK_MAX; j <= z + ChunkManager::LOAD_DISTANCE + Cavern::DISTANCE_CHUNK_MAX; j++) {
			float w = _caverns.noise1dSmoothCosine(i + j * NOISE_STRETCH);
			float global_x = (i + 0.5) * Chunk::SIZE;
			float global_z = (j + 0.5) * Chunk::SIZE;
			glm::vec3	pos = {global_x, heigthAt(global_x, global_z) + 5, global_z};
			new_cache.emplace_back(_caverns, glm::u32vec2(i, j), pos, w);
		}
	}
	_cached_cavern = new_cache;
}
