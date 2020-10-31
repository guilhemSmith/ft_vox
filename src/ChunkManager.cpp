#include "ChunkManager.hpp"

#include <stdlib.h>
#include <time.h>

const glm::u32vec3	ChunkManager::SIZES_VOXELS = {
	16384,
	256,
	16384
};

const glm::u32vec3	ChunkManager::SIZES_CHUNKS = SIZES_VOXELS / Chunk::SIZE;

const float			ChunkManager::VIEW_DISTANCE = 13;	
const float			ChunkManager::LOAD_DISTANCE = 14;	

ChunkManager::ChunkManager(unsigned int seed):
	_world(seed),
	_chunks_loaded(),
	_chunks_visible(),
	_chunks_to_load(),
	_chunks_to_unload(),
	_last_cam_chunk(),
	_mtx(),
	_keep_loading(true),
	_loading_thread(&ChunkManager::_loadRoutine, this)
{}


unsigned int		ChunkManager::_chunkIndex(glm::u32vec3 pos) const {
	return pos.x + pos.y * SIZES_CHUNKS.x + pos.z * SIZES_CHUNKS.x * SIZES_CHUNKS.y;
}

void                    ChunkManager::_chunkRemesh(glm::vec3 pos_chunk) {
    glm::u32vec3 pos(pos_chunk);
    unsigned int index = _chunkIndex(pos_chunk);
    std::array<Chunk*, 6> neighbors;
    auto x_p = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(-1, 0, 0)));
    if (x_p != _chunks_loaded.end())
        neighbors[0] = x_p->second;
    else
        neighbors[0] = nullptr;

    auto x_n = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(1, 0, 0)));
    if (x_n != _chunks_loaded.end())
        neighbors[1] = x_n->second;
    else
        neighbors[1] = nullptr;

    auto y_p = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(0, -1, 0)));
    if (y_p != _chunks_loaded.end())
        neighbors[2] = y_p->second;
    else
        neighbors[2] = nullptr;
    auto y_n = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(0, 1, 0)));
    if (y_n != _chunks_loaded.end())
        neighbors[3] = y_n->second;
    else
        neighbors[3] = nullptr;

    auto z_p = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(0, 0, -1)));
    if (z_p != _chunks_loaded.end())
        neighbors[4] = z_p->second;
    else
        neighbors[4] = nullptr;
    auto z_n = _chunks_loaded.find(_chunkIndex(pos + glm::u32vec3(0, 0, 1)));
    if (z_n != _chunks_loaded.end())
        neighbors[5] = z_n->second;
    else
        neighbors[5] = nullptr;
    _chunks_loaded[index]->remesh(neighbors);
}

void					ChunkManager::_detectVisibleChunks(glm::vec3 pos, glm::vec3 dir) {
	glm::vec3		cam_chunk_pos = pos / static_cast<float>(Chunk::SIZE);

	_chunks_visible.clear();
	_mtx.lock();
	for (auto &loaded_chunk : _chunks_loaded) {
		Chunk* chunk = loaded_chunk.second;
		glm::vec3 pos_chunk = chunk->getPosChunk();
		float dot = glm::dot(dir, pos_chunk + glm::vec3(0.5) - cam_chunk_pos);
		glm::vec3 offset = pos_chunk - cam_chunk_pos;
		float dist = glm::distance(cam_chunk_pos, pos_chunk);
		if (dist < VIEW_DISTANCE && !chunk->is_meshed) {
		    	_chunkRemesh(pos_chunk);
		}
		if ((dist < VIEW_DISTANCE && dot > 0) || dist < 2) {
			_chunks_visible.push_back(chunk);
		}
	}
	_mtx.unlock();
}

void					ChunkManager::_unloadTooFar(glm::vec3 cam_pos_chunk) {
	for (auto &loaded : _chunks_loaded) {
		Chunk* chunk = loaded.second;
		glm::vec3 pos_chunk = chunk->getPosChunk();
		float dist = glm::distance(cam_pos_chunk, pos_chunk);
		if (dist > LOAD_DISTANCE * 2) {
			_chunks_to_unload.push(chunk->getPosChunk());
		}
	}
}

void					ChunkManager::_detectChunkToLoad(glm::u32vec3 cam_chunk_pos) {
	glm::i32vec3	offset = cam_chunk_pos - _last_cam_chunk;
	glm::i32vec3	cam_pos_i = cam_chunk_pos;

	if (offset.x != 0) {
		int dir = glm::sign(offset.x); 
		glm::u32vec3	start = glm::clamp(cam_pos_i + glm::i32vec3(LOAD_DISTANCE * dir, -LOAD_DISTANCE, -LOAD_DISTANCE), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		glm::u32vec3	stop = glm::clamp(cam_pos_i + glm::i32vec3(LOAD_DISTANCE * dir, LOAD_DISTANCE, LOAD_DISTANCE), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		_chunks_to_load.push({start, stop});
	}
	if (offset.y != 0) {
		int dir = glm::sign(offset.y); 
		glm::u32vec3	start = glm::clamp(cam_pos_i + glm::i32vec3(-LOAD_DISTANCE, LOAD_DISTANCE * dir, -LOAD_DISTANCE), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		glm::u32vec3	stop = glm::clamp(cam_pos_i + glm::i32vec3(LOAD_DISTANCE, LOAD_DISTANCE * dir, LOAD_DISTANCE), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		_chunks_to_load.push({start, stop});
	}
	if (offset.z != 0) {
		int dir = glm::sign(offset.z); 
		glm::u32vec3	start = glm::clamp(cam_pos_i + glm::i32vec3(-LOAD_DISTANCE, -LOAD_DISTANCE, LOAD_DISTANCE * dir), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		glm::u32vec3	stop = glm::clamp(cam_pos_i + glm::i32vec3(LOAD_DISTANCE, LOAD_DISTANCE, LOAD_DISTANCE * dir), glm::i32vec3(0,0,0), glm::i32vec3(SIZES_CHUNKS));
		_chunks_to_load.push({start, stop});
	}
}

std::vector<Chunk*>&	ChunkManager::getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir) {
	glm::u32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);
	glm::u32vec3		inbound_cam_chunk_pos = clamp(cam_chunk_pos, {0, 0, 0}, (ChunkManager::SIZES_CHUNKS - glm::u32vec3(1, 1, 1)));

	if (_chunks_loaded.size() == 0) {
		_mtx.lock();
		for (auto x = -LOAD_DISTANCE; x < LOAD_DISTANCE + 1; x++) {
			for (auto y = -LOAD_DISTANCE; y < LOAD_DISTANCE + 1; y++) {
				for (auto z = -LOAD_DISTANCE; z < LOAD_DISTANCE + 1; z++) {
					glm::u32vec3 pos = cam_chunk_pos + glm::u32vec3(x, y, z);
					if (pos.x < SIZES_CHUNKS.x && pos.y < SIZES_CHUNKS.y && pos.z < SIZES_CHUNKS.z) {
						unsigned int index = _chunkIndex(pos);
						_chunks_loaded[index] = new Chunk(_world, pos * Chunk::SIZE);
					}
				}
			}
		}
		_mtx.unlock();
		
		_last_cam_chunk = cam_chunk_pos;
	}

	if (cam_chunk_pos != _last_cam_chunk) {
		_detectChunkToLoad(cam_chunk_pos);
		_unloadTooFar(cam_chunk_pos);
		_last_cam_chunk = cam_chunk_pos;
	}
	if (_chunks_to_unload.size() > 0) {
		while (_chunks_to_unload.size() > 0) {
			glm::u32vec3 pos = _chunks_to_unload.front();
			removeChunk(pos);
			_chunks_to_unload.pop();
		}
	}
	_detectVisibleChunks(cam_pos, cam_dir);
	return _chunks_visible;
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

void				ChunkManager::_loadRoutine(void) {
	while (_keep_loading) {
		if (_chunks_to_load.size() > 0) {
			_mtx.lock();
			std::array<glm::u32vec3, 2> area = _chunks_to_load.front();
			_chunks_to_load.pop();
			_mtx.unlock();
			for (unsigned int x = area[0].x; x < area[1].x + 1; x++) {
				for (unsigned int y = area[0].y; y < area[1].y + 1; y++) {
					for (unsigned int z = area[0].z; z < area[1].z + 1; z++) {
						glm::u32vec3	pos(x, y, z);
						if (pos.x < SIZES_CHUNKS.x && pos.y < SIZES_CHUNKS.y && pos.z < SIZES_CHUNKS.z) {
							unsigned int index = _chunkIndex(pos);
							if (_chunks_loaded.find(index) == _chunks_loaded.end()) {
								_chunks_loaded[index] = new Chunk(_world, pos * Chunk::SIZE);
							}
						}
					}
				}
			}
		}
	}
}