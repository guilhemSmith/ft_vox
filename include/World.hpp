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
	std::unordered_map<unsigned int, Chunk>	_chunks;

	static const glm::u32vec3				_SIZES;
	static const unsigned int				_NOISE_SIZE;

	unsigned int							_chunkIndex(glm::i32vec3 pos) const;

public:
	World(void);
	World(unsigned int seed);

	Chunk&									getChunk(glm::i32vec3 pos);
	Chunk&									removeChunk(glm::i32vec3 pos);

	glm::i32vec3							ChunkCoord(glm::vec3 world_pos) const;
};

#endif