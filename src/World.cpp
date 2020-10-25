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
	_chunks_loaded(),
	_chunks_visible()
{}

World::World(void):
	_seed(time(NULL)),
	_noise_height(World::NOISE_SIZE, _seed),
	_chunks_loaded(),
	_chunk_last_loaded()
{}


unsigned int		World::_chunkIndex(glm::u32vec3 pos) const {
	return pos.x + pos.y * SIZES_CHUNKS.x + pos.z * SIZES_CHUNKS.x * SIZES_CHUNKS.y;
}

void				World::_loadNeighbour(glm::vec3 pos, glm::vec3 dir, std::vector<Chunk*>& container) {
	glm::vec3 pos_neigh = pos + dir;
	glm::vec3 cam_chunk = _last_cam_chunk;
	float dist = glm::distance(cam_chunk * dir, pos_neigh * dir);
	if (dist < 6.0) {
		Chunk*	chunk = getChunk(pos_neigh);
		if (chunk != NULL && !chunk->is_visible) {
			chunk->is_visible = true;
			container.push_back(chunk);
		}
	}
}

void				World::_loadNewChunk(void) {
	std::vector<Chunk*>		new_load = std::vector<Chunk*>();

	if (_chunk_last_loaded.size() > 0) {
		for (auto &loaded_chunk : _chunk_last_loaded) {
			_loadNeighbour(loaded_chunk->getPosChunk(), {1, 0, 0}, new_load);
			_loadNeighbour(loaded_chunk->getPosChunk(), {-1, 0, 0}, new_load);
			_loadNeighbour(loaded_chunk->getPosChunk(), {0, 1, 0}, new_load);
			_loadNeighbour(loaded_chunk->getPosChunk(), {0, -1, 0}, new_load);
			_loadNeighbour(loaded_chunk->getPosChunk(), {0, 0, 1}, new_load);
			_loadNeighbour(loaded_chunk->getPosChunk(), {0, 0, -1}, new_load);
		}
		_chunk_last_loaded = new_load;
	}
}

std::vector<Chunk*>&	World::getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir) {
	glm::u32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);
	glm::u32vec3		inbound_cam_chunk_pos = clamp(cam_chunk_pos, {0, 0, 0}, (World::SIZES_CHUNKS - glm::u32vec3(1, 1, 1)));

	if (_chunks_visible.size() == 0) {
		// Chunk*	chunk = getChunk(inbound_cam_chunk_pos);
		
		// if (chunk != NULL) {
		// 	chunk->is_visible = true;
		// 	_chunk_last_loaded.push_back(chunk);
		// 	_chunks_visible.push_back(chunk);
		// }
	}
	if (cam_chunk_pos != _last_cam_chunk) {
		Chunk*	chunk = getChunk(inbound_cam_chunk_pos);
		_chunks_visible = std::vector<Chunk*>();
		
		if (chunk != NULL) {
			chunk->is_visible = true;
			_chunk_last_loaded.push_back(chunk);
			_chunks_visible.push_back(chunk);
		}
		_last_cam_chunk = cam_chunk_pos;
		// glm::vec3		chunk_offset = static_cast<glm::vec3>(cam_chunk_pos) - static_cast<glm::vec3>(_last_cam_chunk);
		// std::vector<Chunk*>	to_clean = std::vector<Chunk*>();

		// for (auto &chunk : _chunks_visible) {
		// 	glm::vec3	chunk_pos = chunk->getPosChunk();
		// 	glm::vec3	dist_vec = chunk->getPosChunk() - cam_chunk_pos;
		// 	float dist = glm::distance(static_cast<glm::vec3>(cam_chunk_pos) * dist_vec, chunk_pos * dist_vec);
		// 	float dot = glm::dot(chunk_offset, dist_vec);
		// 	if (dist > 6.0 && dot > 0.0) {
		// 		to_clean.push_back(chunk);
		// 	}
		// 	if (dist > 5.0 && dot < 0.0) {
		// 		_chunk_last_loaded.push_back(chunk);
		// 	}
		// }

		// // if (to_clean.size() > 0) {
		// // 	std::vector<Chunk*>::iterator remove_it = _chunks_visible.begin();
		// // 	for (auto &chunk : to_clean) {
		// // 		remove_it = std::remove(remove_it, _chunks_visible.end(), chunk);
		// // 		_chunks_loaded.erase(_chunkIndex(chunk->getPosChunk()));
		// // 		delete chunk;
		// // 	}
		// // 	_chunks_visible.erase(remove_it, _chunks_visible.end());
		// // 	_last_cam_chunk = cam_chunk_pos;
		// // }
	}
	else {
		_loadNewChunk();
		for (auto loaded_chunk : _chunk_last_loaded) {
			_chunks_visible.push_back(loaded_chunk);
		}

	}
	return _chunks_visible;
}

Chunk*				World::getChunk(glm::u32vec3 pos) {
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

void				World::removeChunk(glm::u32vec3 pos) {
	unsigned int index = _chunkIndex(pos);
	try {
		_chunks_loaded.erase(index);
	}
	catch (std::out_of_range oor) {
		// Can't unload not loaded chunk
	}
}