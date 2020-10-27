#include "World.hpp"
#include "ChunkManager.hpp"
#include "Chunk.hpp"


const unsigned int	World::NOISE_STRETCH = 64;

const unsigned int	World::NOISE_SIZE = ChunkManager::SIZES_VOXELS.x / NOISE_STRETCH * ChunkManager::SIZES_VOXELS.z / NOISE_STRETCH;

World::World(unsigned int seed): _seed(seed), _heightmap(NOISE_SIZE), _biomes(NOISE_SIZE), _caverns(NOISE_SIZE) {}

bool				World::fillChunk(std::array<std::array<std::array<char, 32>, 32>, 32>& voxels, glm::vec3 pos) const {
	bool	empty = true;
	for (int z = 0; z < Chunk::SIZE; z++) {
		for (int x = 0; x < Chunk::SIZE; x++) {
			double height = _heightmap.perlin2d(4, 1.0, 0.5, (x + pos.x) / NOISE_STRETCH, (z + pos.z) / NOISE_STRETCH);
			height = height * ChunkManager::SIZES_VOXELS.y;
			for (int y = 0; y < Chunk::SIZE; y++) {
				if (y + pos.y > height) {
					voxels[x][y][z] = Chunk::Voxel::Empty;
				}
				else {
					voxels[x][y][z] = Chunk::Voxel::Rock;
					empty = false;
				}
			}
		}
	}
	return empty;
}