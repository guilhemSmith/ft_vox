#ifndef WORLD_HPP
# define WORLD_HPP

# include "Noise.hpp"
# include "Chunk.hpp"
# include <glm/glm.hpp>
# include <unordered_map>

class World {
private:
	unsigned int const						_seed;
	Noise									_noise_height;
	std::unordered_map<unsigned int, Chunk*>	_chunks;

	unsigned int							_chunkIndex(glm::u32vec3 pos) const;

public:
	World(void);
	World(unsigned int seed);

	std::vector<Chunk*>						getChunksFromPos(glm::vec3 cam_pos);
	Chunk*									getChunk(glm::u32vec3 pos);
	void									removeChunk(glm::u32vec3 pos);

	glm::u32vec3							ChunkCoord(glm::vec3 world_pos) const;

	static const glm::u32vec3				SIZES_VOXELS;
	static const glm::u32vec3				SIZES_CHUNKS;
	static const unsigned int				NOISE_STRETCH;
	static const unsigned int				NOISE_SIZE;
};

#endif