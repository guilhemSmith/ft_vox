#ifndef CHUNK_MANAGER_HPP
# define CHUNK_MANAGER_HPP

# include "World.hpp"
# include "Chunk.hpp"
# include <glm/glm.hpp>
# include <unordered_map>
# include <vector>

class ChunkManager {
private:
	World									                    _world;
	std::unordered_map<unsigned int, std::shared_ptr<Chunk>>	_chunks_loaded;
	std::vector<std::weak_ptr<Chunk>>						    _chunks_visible;
	std::vector<glm::u32vec3>				                    _chunks_to_load;
	std::vector<glm::u32vec3>				                    _chunks_to_unload;
	glm::u32vec3							                    _last_cam_chunk;

	unsigned int							                    _chunkIndex(glm::u32vec3 pos) const;
	void									                    _detectVisibleChunks(glm::vec3 pos, glm::vec3 dir);
	void									                    _unloadTooFar(glm::vec3 cam_pos_chunk);
	void									                    _detectChunkToLoad(glm::u32vec3 cam_chunk_pos);
	void                                                        _chunkRemesh(glm::vec3);

public:
	ChunkManager(unsigned int seed);

	std::vector<std::weak_ptr<Chunk>>&					        getChunksFromPos(glm::vec3 cam_pos, glm::vec3 cam_dir);
	void									                    removeChunk(glm::u32vec3 pos);

	static const glm::u32vec3				                    SIZES_VOXELS;
	static const glm::u32vec3				                    SIZES_CHUNKS;
	static const float						                    VIEW_DISTANCE;
};

#endif