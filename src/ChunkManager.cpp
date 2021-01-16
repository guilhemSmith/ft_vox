#include "ChunkManager.hpp"

const glm::u32vec3	ChunkManager::SIZES_VOXELS = {
	16384,
	256,
	16384
};

const glm::u32vec3	ChunkManager::SIZES_CHUNKS = SIZES_VOXELS / Chunk::SIZE;

const float			ChunkManager::VIEW_DISTANCE = 9;
const float			ChunkManager::LOAD_DISTANCE = 11;

ChunkManager::ChunkManager(unsigned int seed):
	_world(seed),
	_chunks_loaded(),
	_chunks_visible(),
	_chunks_to_load(),
	_chunks_to_unload(),
	_last_cam_chunk(),
	_mtx(),
	_keep_loading(true),
	_is_loading(true),
	_loading_thread(&ChunkManager::_loadRoutine, this)
{}

ChunkManager::~ChunkManager() {
	_keep_loading = false;
	_chunks_loaded.clear();
}

void 				ChunkManager::procNeightborsReload(glm::u32vec3 hit) {
    glm::i32vec3			chunk_pos = hit / Chunk::SIZE;
    glm::i32vec3			voxel_pos = hit % Chunk::SIZE;
	if (voxel_pos.x == 0) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(-1, 0, 0);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
	if (voxel_pos.x == Chunk::SIZE - 1) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(1, 0, 0);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
	if (voxel_pos.y == 0) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(0, -1, 0);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
	if (voxel_pos.y == Chunk::SIZE - 1) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(0, 1, 0);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
	if (voxel_pos.z == 0) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(0, 0, -1);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
	if (voxel_pos.z == Chunk::SIZE - 1) {
		glm::u32vec3 n_pos = chunk_pos + glm::i32vec3(0, 0, 1);
		auto ptr = _chunks_loaded.find(chunkIndex(n_pos));
		if (ptr != _chunks_loaded.end())
			_chunkRemesh(n_pos);
	}
}

bool                ChunkManager::tryDeleteVoxel(glm::u32vec3 pos) {
    glm::u32vec3			chunk_pos = pos / Chunk::SIZE;
    glm::u32vec3			voxel_pos = pos % Chunk::SIZE;
    unsigned int			index = chunkIndex(chunk_pos);
    try {
        std::shared_ptr<Chunk> chunk = _chunks_loaded.at(index);
        if (chunk->hasVoxelAt(voxel_pos.x, voxel_pos.y, voxel_pos.z)) {
            chunk->deleteVoxel(voxel_pos);
            _chunkRemesh(chunk_pos);
            return true;
        }
    }
    catch (std::out_of_range oor) {
        return false;
    }

    return false;
}

glm::vec3			ChunkManager::spawnPos(void) const {
	return glm::vec3(SIZES_VOXELS.x / 2, _world.heigthAt(SIZES_VOXELS.x / 2, SIZES_VOXELS.x / 2) + 2.5, SIZES_VOXELS.z / 2);
}

unsigned int		ChunkManager::chunkIndex(glm::u32vec3 pos) const {
	return pos.x + pos.y * SIZES_CHUNKS.x + pos.z * SIZES_CHUNKS.x * SIZES_CHUNKS.y;
}

bool				ChunkManager::_chunkRemesh(glm::vec3 pos_chunk) {
    glm::i32vec3 pos(pos_chunk);
    unsigned int index = chunkIndex(pos_chunk);
    std::array<std::shared_ptr<Chunk>, 6> neighbors;
	if (pos.x > 0.0) {
		auto x_p = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(-1, 0, 0)));
		if (x_p != _chunks_loaded.end())
			neighbors[0] = x_p->second;
		else 
			return false;
	}
	else
		neighbors[0] = nullptr;

	if (pos.x < SIZES_CHUNKS.x) {
		auto x_n = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(1, 0, 0)));
		if (x_n != _chunks_loaded.end())
			neighbors[1] = x_n->second;
		else
			return false;
	}
	else
        neighbors[1] = nullptr;

	if (pos.y > 0.0) {
		auto y_p = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(0, -1, 0)));
		if (y_p != _chunks_loaded.end())
			neighbors[2] = y_p->second;
		else
			return false;
	}
	else
        neighbors[2] = nullptr;

	if (pos.y < SIZES_CHUNKS.y) {
		auto y_n = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(0, 1, 0)));
		if (y_n != _chunks_loaded.end())
			neighbors[3] = y_n->second;
		else
			return false;
	}
	else
        neighbors[3] = nullptr;


	if (pos.z > 0.0) {
		auto z_p = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(0, 0, -1)));
		if (z_p != _chunks_loaded.end())
			neighbors[4] = z_p->second;
		else
			return false;
	}
	else
        neighbors[4] = nullptr;

	if (pos.z < SIZES_CHUNKS.z) {
		auto z_n = _chunks_loaded.find(chunkIndex(pos + glm::i32vec3(0, 0, 1)));
		if (z_n != _chunks_loaded.end())
			neighbors[5] = z_n->second;
		else
			return false;
	}
	else
        neighbors[5] = nullptr;
	
    _chunks_loaded[index]->remesh(neighbors);
	return true;
}

void					ChunkManager::_detectVisibleChunks(glm::vec3 pos, glm::vec3 dir) {
	glm::vec3		cam_chunk_pos = pos / static_cast<float>(Chunk::SIZE);

	_chunks_visible.clear();
	_mtx.lock();
	for (auto &loaded_chunk : _chunks_loaded) {
        glm::vec3 pos_chunk = loaded_chunk.second->getPosChunk();
        float dot = glm::dot(dir, pos_chunk + glm::vec3(0.5) - cam_chunk_pos);
        glm::vec3 offset = glm::abs(pos_chunk - cam_chunk_pos);
        float dist = glm::distance(cam_chunk_pos, pos_chunk);
		// float dist = glm::max(offset.x, glm::max(offset.y, offset.z));
        if (dist < VIEW_DISTANCE && !loaded_chunk.second->is_meshed) {
            if (!_chunkRemesh(pos_chunk)) {
				continue;
			}
        }
        if ((dist < VIEW_DISTANCE && dot > 0) || dist < 2) {
            _chunks_visible.emplace_back(loaded_chunk.second);
        }
	}
	_mtx.unlock();
}

void					ChunkManager::_unloadTooFar(glm::vec3 cam_pos_chunk) {
	if (_chunks_to_unload.size() == 0) {
		_mtx.lock();
		for (auto &loaded : _chunks_loaded) {
			glm::vec3 pos_chunk = loaded.second->getPosChunk();
			glm::vec3 offset = glm::abs(pos_chunk - cam_pos_chunk);
			if (offset.x > LOAD_DISTANCE + 2 || offset.y > LOAD_DISTANCE + 2 || offset.z > LOAD_DISTANCE + 2) {
				_chunks_to_unload.emplace(loaded.second->getPosChunk());
			}
		}
		_mtx.unlock();
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

void								ChunkManager::loadInitialChunks(glm::vec3 cam_pos) {
	glm::u32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);
	glm::i32vec3		cam_pos_i = cam_chunk_pos;

	_world.cacheCavernsAround(cam_chunk_pos.x, cam_chunk_pos.z);
	glm::u32vec3		start = glm::clamp(
		cam_pos_i + glm::i32vec3(-LOAD_DISTANCE, -LOAD_DISTANCE, -LOAD_DISTANCE),
		glm::i32vec3(0,0,0),
		glm::i32vec3(SIZES_CHUNKS)
	);
	glm::u32vec3		stop = glm::clamp(
		cam_pos_i + glm::i32vec3(LOAD_DISTANCE, LOAD_DISTANCE, LOAD_DISTANCE),
		glm::i32vec3(0,0,0),
		glm::i32vec3(SIZES_CHUNKS)
	);
	_chunks_to_load.push({start, stop});
	_last_cam_chunk = cam_chunk_pos;
}

std::vector<std::weak_ptr<Chunk>>&	ChunkManager::getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir) {
	glm::i32vec3		cam_chunk_pos = cam_pos / static_cast<float>(Chunk::SIZE);
	glm::i32vec3		inbound_cam_chunk_pos_signed = clamp(cam_chunk_pos, {0, 0, 0}, (glm::i32vec3(SIZES_CHUNKS) - glm::i32vec3(1, 1, 1)));
	glm::u32vec3		inbound_cam_chunk_pos = inbound_cam_chunk_pos_signed;

	if (inbound_cam_chunk_pos != _last_cam_chunk) {
		_world.cacheCavernsAround(inbound_cam_chunk_pos.x, inbound_cam_chunk_pos.z);
		_detectChunkToLoad(inbound_cam_chunk_pos);
		_unloadTooFar(inbound_cam_chunk_pos);
		_last_cam_chunk = inbound_cam_chunk_pos;
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
	unsigned int index = chunkIndex(pos);
	try {
		_mtx.lock();
		_chunks_loaded.erase(index);
		_mtx.unlock();
	}
	catch (std::out_of_range oor) {
		//std::cout << "Trying to free unloaded chunk." << std::endl;
	}
}


void				ChunkManager::_loadRoutine(void) {
	while (_keep_loading) {
		if (_chunks_to_load.size() > 0) {
			_is_loading = true;
			_mtx.lock();
			std::array<glm::u32vec3, 2> area = _chunks_to_load.front();
			_chunks_to_load.pop();
			_mtx.unlock();
			for (unsigned int x = area[0].x; x < area[1].x + 1; x++) {
				for (unsigned int z = area[0].z; z < area[1].z + 1; z++) {
					if (x < SIZES_CHUNKS.x && z < SIZES_CHUNKS.z) {
						_world.cacheAmplitudeAt(x, z);
						_world.cacheBiomeAt(x, z);
						_world.cacheHolesNear(x, z);
						for (unsigned int y = area[0].y; y < area[1].y + 1; y++) {
							if (y < SIZES_CHUNKS.y) {
								glm::u32vec3	pos(x, y, z);
								unsigned int index = chunkIndex(pos);
								if (_chunks_loaded.find(index) == _chunks_loaded.end()) {
									_mtx.lock();
									_chunks_loaded[index] = std::make_shared<Chunk>(_world, pos * Chunk::SIZE);
									_mtx.unlock();
								}
							}
						}
					}
				}
			}
		}
		else {
			_is_loading = false;
		}
	}
}

bool				ChunkManager::isLoading(void) const {
	return _is_loading;
}