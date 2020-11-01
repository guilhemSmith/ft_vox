#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"
# include "World.hpp"
# include <memory>

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
			Grass = 3,
			Sand = 4
		};

		Chunk(const World& world, glm::vec3 pos);
		~Chunk();
		bool 				is_empty;
		bool                is_meshed;
		void 				draw();
		void 				remesh(std::array<std::shared_ptr<Chunk>, 6>&);
		glm::vec3 			getPos();
		glm::u32vec3		getPosChunk();
		bool 				hasVoxelAt(int x, int y, int z) const;

		static const unsigned int SIZE;
};

#endif
