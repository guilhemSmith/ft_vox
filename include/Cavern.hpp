#ifndef CAVERN_HPP
# define CAVERN_HPP

# include <vector>
# include <glm/glm.hpp>
# include "Noise.hpp"

class Cavern
{
private:
	glm::u32vec2				_chunk_pos;
	glm::vec3					_pos;
	std::vector<glm::vec3>		_holes;
public:
	Cavern(Noise& noise, glm::u32vec2 chunk_pos, glm::vec3 pos, float w);

	const glm::u32vec2&			chunkPos(void) const;
	void						selectHoles(float x, float z, std::vector<glm::vec3>& selected) const;

	static const unsigned int	SIZE;
	static const float			STEP;
	static const float			HOLE_SIZE;
	static const unsigned int	DISTANCE_CHUNK_MAX;
};


#endif