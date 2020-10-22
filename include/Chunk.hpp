#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"
# include "Noise.hpp"

class Chunk {
	private:
		std::vector<char> 	_cubes;
		Mesh 				_mesh;
		glm::vec3 			_pos;
		
	public:
		Chunk(const Noise& heights, glm::u32vec3 pos);
		void 				draw();
		void 				remesh();
		const glm::vec3 	getPos();
		
		static const unsigned int	SIZE;
};

#endif
