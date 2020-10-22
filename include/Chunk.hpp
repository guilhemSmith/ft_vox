#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"

class Chunk {
	private:
    std::array<std::array<std::array<char, 16>, 16>, 16>				_cubes;
		Mesh 				_mesh;
		glm::vec3 			_pos;
		
	public:
		Chunk(glm::vec3);
		bool 				is_empty;
		void 				draw();
		void 				remesh();
		glm::vec3 	getPos();
};

#endif
