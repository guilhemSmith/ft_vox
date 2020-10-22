#include "World.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	World::SIZES_VOXELS = {
	16384,
	256,
	16384
};

const glm::u32vec3	World::SIZES_CHUNKS = {
	SIZES_VOXELS.x / Chunk::SIZE,
	SIZES_VOXELS.y / Chunk::SIZE,
	SIZES_VOXELS.z / Chunk::SIZE
};

const unsigned int	World::NOISE_STRETCH = 64;

const unsigned int	World::NOISE_SIZE = SIZES_VOXELS.x / NOISE_STRETCH * SIZES_VOXELS.z / NOISE_STRETCH;

World::World(unsigned int seed):
	_seed(seed),
	_noise_height(World::NOISE_SIZE, _seed),
	_chunks()
{}

World::World(void):
	_seed(time(NULL)),
	_noise_height(World::NOISE_SIZE, _seed),
	_chunks()
{}


unsigned int		World::_chunkIndex(glm::u32vec3 pos) const {
	return pos.x + pos.y * SIZES_CHUNKS.x + pos.z * SIZES_CHUNKS.x * SIZES_CHUNKS.y;
}

Chunk*				World::getChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		return _chunks.at(index);
	}
	catch (std::out_of_range oor) {
		_chunks[index] = new Chunk(_noise_height, pos);
		return _chunks[index];
	}
}

void				World::removeChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		_chunks.erase(index);
	}
	catch (std::out_of_range oor) {
		// Can't unload not loaded chunk
	}
}

glm::u32vec3		World::ChunkCoord(glm::vec3 world_pos) const {
	return {
		static_cast<unsigned int>(world_pos.x / Chunk::SIZE),
		static_cast<unsigned int>(world_pos.y / Chunk::SIZE),
		static_cast<unsigned int>(world_pos.z / Chunk::SIZE),
	};
}