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
	World														_world;
	std::unordered_map<unsigned int, std::shared_ptr<Chunk>>	_chunks_loaded;
	std::vector<std::weak_ptr<Chunk>>							_chunks_visible;
	std::queue<std::array<glm::u32vec3, 2>>						_chunks_to_load;
	std::queue<glm::u32vec3>									_chunks_to_unload;
	glm::u32vec3												_last_cam_chunk;
	std::mutex													_mtx;
	bool														_keep_loading;
	std::thread													_loading_thread;

	void									                    _detectVisibleChunks(glm::vec3 pos, glm::vec3 dir);
	void									                    _unloadTooFar(glm::vec3 cam_pos_chunk);
	void									                    _detectChunkToLoad(glm::u32vec3 cam_chunk_pos);
	bool                                                        _chunkRemesh(glm::vec3);

	void														_loadRoutine(void);

public:
	ChunkManager(unsigned int seed);
	~ChunkManager();

	bool                                                        tryDeleteVoxel(glm::vec3 pos);
    unsigned int							                    chunkIndex(glm::u32vec3 pos) const;
	void														loadInitialChunks(glm::vec3 cam_pos);
	std::vector<std::weak_ptr<Chunk>>&							getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir);
	glm::vec3													spawnPos(void) const;
	void														removeChunk(glm::u32vec3 pos);

	static const glm::u32vec3									SIZES_VOXELS;
	static const glm::u32vec3									SIZES_CHUNKS;
	static const float											VIEW_DISTANCE;
	static const float											LOAD_DISTANCE;
};

#endif