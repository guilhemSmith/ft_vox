#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"
# include "Noise.hpp"

class Chunk {
	private:

    std::array<std::array<std::array<char, 32>, 32>, 32>				_cubes;
		Mesh 				_mesh;
		glm::vec3 			_pos;
		
	public:
		enum Voxel {
			Empty = 0,
			Rock = 1,
			Dirt = 2,
			Grass = 3
		};

		Chunk(const Noise& heights, glm::vec3);
		bool 				is_empty;
		void 				draw();
		void 				remesh();
		glm::vec3 			getPos();

		static const unsigned int SIZE;
};

#endif
