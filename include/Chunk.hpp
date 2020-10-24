#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"
# include "Noise.hpp"

class Chunk {
	private:
	enum Voxel {
		Empty = 'e',
		rock = 'a'
	};

    std::array<std::array<std::array<char, 16>, 16>, 16>				_cubes;
		Mesh 				_mesh;
		glm::vec3 			_pos;
		
	public:
		Chunk(glm::vec3);
		bool 				is_empty;
		void 				draw();
		void 				remesh();
		glm::vec3 			getPos();

		static const unsigned int SIZE;
};

#endif
