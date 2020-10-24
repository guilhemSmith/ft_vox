#include "World.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	World::SIZES_VOXELS = {
	16384,
	256,
	16384
};

const glm::u32vec3	World::SIZES_CHUNKS = SIZES_VOXELS / Chunk::SIZE;

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

std::vector<Chunk*>&	World::getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir) {
	glm::u32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);

	if (cam_chunk_pos != _last_cam_chunk) {
		_loaded = std::vector<Chunk*>();
		_last_cam_chunk = cam_chunk_pos;
		for (auto x = -5; x < 6; x++) {
			for (auto y = -5; y < 6; y++) {
				for (auto z = -5; z < 6; z++) {
					// glm::vec3	chunk_pos = cam_chunk_pos + glm::u32vec3(x, y, z);
					// if (glm::length(glm::vec3(x, y, z)) > 2 && glm::dot(cam_dir, glm::vec3(x, y, z)) > 0) {
						Chunk*	chunk = getChunk(cam_chunk_pos + glm::u32vec3(x, y, z));
						if (chunk != NULL) {
							_loaded.push_back(chunk);
						}
					// }
				}
			}
		}
	}

	return _loaded;
}

Chunk*				World::getChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);

	if (pos.x < SIZES_CHUNKS.x && pos.y < SIZES_CHUNKS.y && pos.z < SIZES_CHUNKS.z) {
		try {
			return _chunks.at(index);
		}
		catch (std::out_of_range oor) {
			_chunks[index] = new Chunk(_noise_height, pos * Chunk::SIZE);
			_chunks[index]->remesh();
			return _chunks[index];
		}
	}
	else {
		return NULL;
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