#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "World.hpp"
# include "Chunk.hpp"
# include <glm/glm.hpp>
# include <unordered_map>
# include <vector>
# include <queue>
# include <mutex>
# include <thread>

class ChunkManager {
private:
	World									_world;
	std::unordered_map<unsigned int, Chunk*>	_chunks_loaded;
	std::vector<Chunk*>						_chunks_visible;
	std::queue<std::array<glm::u32vec3, 2>>	_chunks_to_load;
	std::queue<glm::u32vec3>				_chunks_to_unload;
	glm::u32vec3							_last_cam_chunk;
	std::mutex								_mtx;
	bool									_keep_loading;
	std::thread								_loading_thread;

	unsigned int							_chunkIndex(glm::u32vec3 pos) const;
	void									_detectVisibleChunks(glm::vec3 pos, glm::vec3 dir);
	void									_unloadTooFar(glm::vec3 cam_pos_chunk);
	void									_detectChunkToLoad(glm::u32vec3 cam_chunk_pos);
	void                                    _chunkRemesh(glm::vec3);

	void									_loadRoutine(void);

public:
	ChunkManager(unsigned int seed);

	std::vector<Chunk*>&					getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir);
	// Chunk*									getChunk(glm::u32vec3 pos);
	void									removeChunk(glm::u32vec3 pos);

	static const glm::u32vec3				SIZES_VOXELS;
	static const glm::u32vec3				SIZES_CHUNKS;
	static const float						VIEW_DISTANCE;
	static const float						LOAD_DISTANCE;
};

#endif