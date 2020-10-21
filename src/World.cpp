#include "World.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	World::_SIZES = {
	16384 / Chunk::SIZE,
	256 / Chunk::SIZE,
	16384 / Chunk::SIZE
};

const unsigned int	World::_NOISE_SIZE = 256 * 256;

World::World(unsigned int seed):
	_seed(seed),
	_noise_height(World::_NOISE_SIZE, _seed),
	_chunks()
{}

World::World(void):
	_seed(time(NULL)),
	_noise_height(World::_NOISE_SIZE, _seed),
	_chunks()
{}


unsigned int		World::_chunkIndex(glm::i32vec3 pos) const {
	return pos.x + pos.y * _SIZES.x + pos.z * _SIZES.x * _SIZES.y;
}

Chunk&				World::getChunk(glm::i32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		return _chunks.at(index);
	}
	catch (std::out_of_range oor) {
		// TODO: load chunk from noises
	}
}

Chunk&				World::removeChunk(glm::i32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		_chunks.erase(index);
	}
	catch (std::out_of_range oor) {
		// Can't unload not loaded chunk
	}
}

glm::i32vec3		World::ChunkCoord(glm::vec3 world_pos) const {
	return {
		static_cast<int>(world_pos.x / Chunk::SIZE),
		static_cast<int>(world_pos.y / Chunk::SIZE),
		static_cast<int>(world_pos.z / Chunk::SIZE),
	};
}