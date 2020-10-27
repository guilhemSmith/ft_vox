#include "ChunkManager.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	ChunkManager::SIZES_VOXELS = {
	16384,
	256,
	16384
};

const glm::u32vec3	ChunkManager::SIZES_CHUNKS = SIZES_VOXELS / Chunk::SIZE;

const unsigned int	ChunkManager::NOISE_STRETCH = 64;

const unsigned int	ChunkManager::NOISE_SIZE = SIZES_VOXELS.x / NOISE_STRETCH * SIZES_VOXELS.z / NOISE_STRETCH;

const float			ChunkManager::VIEW_DISTANCE = 7;	

ChunkManager::ChunkManager(unsigned int seed):
	_seed(seed),
	_noise_height(ChunkManager::NOISE_SIZE, _seed),
	_chunks_loaded(),
	_chunks_visible(),
	_chunks_to_load(),
	_chunks_to_unload()
{}

ChunkManager::ChunkManager(void):
	_seed(time(NULL)),
	_noise_height(ChunkManager::NOISE_SIZE, _seed),
	_chunks_loaded(),
	_chunks_to_load()
{}


unsigned int		ChunkManager::_chunkIndex(glm::u32vec3 pos) const {
	return pos.x + pos.y * SIZES_CHUNKS.x + pos.z * SIZES_CHUNKS.x * SIZES_CHUNKS.y;
}

void					ChunkManager::_detectVisibleChunks(glm::vec3 pos, glm::vec3 dir) {
	glm::vec3		cam_chunk_pos = pos / static_cast<float>(Chunk::SIZE);

	_chunks_visible.clear();
	for (auto &loaded_chunk : _chunks_loaded) {
		Chunk* chunk = loaded_chunk.second;
		glm::vec3 pos_chunk = chunk->getPosChunk();
		float dot = glm::dot(dir, pos_chunk + glm::vec3(0.5) - cam_chunk_pos);
		glm::vec3 offset = pos_chunk - cam_chunk_pos;
		float dist = glm::max(glm::length(offset * glm::vec3(1,0,0)), glm::max(glm::length(offset * glm::vec3(0,1,0)), glm::length(offset * glm::vec3(0,0,1))));
		if ((dist < VIEW_DISTANCE && dot > 0) || dist < 2) {
			_chunks_visible.push_back(chunk);
		}
	}
}

void					ChunkManager::_unloadTooFar(glm::vec3 cam_pos_chunk) {
	for (auto &loaded : _chunks_loaded) {
		Chunk* chunk = loaded.second;
		glm::vec3 pos_chunk = chunk->getPosChunk();
		float dist = glm::distance(cam_pos_chunk, pos_chunk);
		if (dist > VIEW_DISTANCE * 2) {
			_chunks_to_unload.push_back(chunk->getPosChunk());
		}
	}
}

void					ChunkManager::_detectChunkToLoad(glm::u32vec3 cam_chunk_pos) {
	glm::i32vec3	offset = cam_chunk_pos - _last_cam_chunk;

	for (auto i = -VIEW_DISTANCE; i < VIEW_DISTANCE + 1; i++) {
		for (auto j = -VIEW_DISTANCE; j < VIEW_DISTANCE + 1; j++) {
			if (offset.x != 0) {
				glm::u32vec3 chunk_pos = cam_chunk_pos + glm::u32vec3(VIEW_DISTANCE * offset.x, i, j);
				if (_chunks_loaded.find(_chunkIndex(chunk_pos)) == _chunks_loaded.end()) {
					_chunks_to_load.push_back(chunk_pos);
				}
			}
			if (offset.y != 0) {
				glm::u32vec3 chunk_pos = cam_chunk_pos + glm::u32vec3(i, VIEW_DISTANCE * offset.y, j);
				if (_chunks_loaded.find(_chunkIndex(chunk_pos)) == _chunks_loaded.end()) {
					_chunks_to_load.push_back(chunk_pos);
				}
			}
			if (offset.z != 0) {
				glm::u32vec3 chunk_pos = cam_chunk_pos + glm::u32vec3(i, j, VIEW_DISTANCE * offset.z);
				if (_chunks_loaded.find(_chunkIndex(chunk_pos)) == _chunks_loaded.end()) {
					_chunks_to_load.push_back(chunk_pos);
				}
			}
		}
	}
}

std::vector<Chunk*>&	ChunkManager::getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir) {
	glm::u32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);
	glm::u32vec3		inbound_cam_chunk_pos = clamp(cam_chunk_pos, {0, 0, 0}, (ChunkManager::SIZES_CHUNKS - glm::u32vec3(1, 1, 1)));

	if (_chunks_loaded.size() == 0) {
		for (auto x = -VIEW_DISTANCE - 1; x < VIEW_DISTANCE + 2; x++) {
			for (auto y = -VIEW_DISTANCE - 1; y < VIEW_DISTANCE + 2; y++) {
				for (auto z = -VIEW_DISTANCE - 1; z < VIEW_DISTANCE + 2; z++) {
					glm::u32vec3 chunk_pos = cam_chunk_pos + glm::u32vec3(x, y, z);
					_chunks_to_load.push_back(chunk_pos);
				}
			}
		}
	}

	if (cam_chunk_pos != _last_cam_chunk) {
		_detectChunkToLoad(cam_chunk_pos);
		_unloadTooFar(cam_chunk_pos);
		_last_cam_chunk = cam_chunk_pos;
	}
	if (_chunks_to_unload.size() > 0) {
		int count = 0;
		for (auto &pos : _chunks_to_unload) {
			removeChunk(pos);
			count++;
		}
		_chunks_to_unload.clear();
	}
	if (_chunks_to_load.size() > 0) {
		for (auto &pos : _chunks_to_load) {
			if (pos.x < SIZES_CHUNKS.x && pos.y < SIZES_CHUNKS.y && pos.z < SIZES_CHUNKS.z) {
				unsigned int index = _chunkIndex(pos);
				_chunks_loaded[index] = new Chunk(_noise_height, pos * Chunk::SIZE);
				_chunks_loaded[index]->remesh();
			}
		}
		_chunks_to_load.clear();
	}
	_detectVisibleChunks(cam_pos, cam_dir);
	return _chunks_visible;
}

Chunk*				ChunkManager::getChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);

	if (pos.x < SIZES_CHUNKS.x && pos.y < SIZES_CHUNKS.y && pos.z < SIZES_CHUNKS.z) {
		try {
			return _chunks_loaded.at(index);
		}
		catch (std::out_of_range oor) {
			_chunks_loaded[index] = new Chunk(_noise_height, pos * Chunk::SIZE);
			_chunks_loaded[index]->remesh();
			return _chunks_loaded[index];
		}
	}
	else {
		return NULL;
	}
}

void				ChunkManager::removeChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		delete _chunks_loaded.at(index);
		_chunks_loaded.erase(index);
	}
	catch (std::out_of_range oor) {
		//std::cout << "Trying to free unloaded chunk." << std::endl;
	}
}