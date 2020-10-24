#include "Chunk.hpp"
#include "World.hpp"
#include <glm/glm.hpp>

const unsigned int			Chunk::SIZE = 32;

Chunk::Chunk(const Noise& heights, glm::vec3 pos) : _pos(pos) {
	is_empty = true;
	for (int z = 0; z < SIZE; z++) {
		for (int x = 0; x < SIZE; x++) {
			double height = heights.perlin2d(4, 1.0, 0.5, (x + pos.x) / World::NOISE_STRETCH, (z + pos.z) / World::NOISE_STRETCH);
			height = height * World::SIZES_VOXELS.y;
			for (int y = 0; y < SIZE; y++) {
				if (y + pos.y > height) {
					_cubes[x][y][z] = Voxel::Empty;
				}
				else {
					_cubes[x][y][z] = Voxel::Rock;
					is_empty = false;
				}
			}
		}
	}
}

void 		Chunk::draw() {
	_mesh.draw();
}

void 		Chunk::remesh() {
	//TODO init mesh with voxel data
	_mesh = Mesh(_cubes, _pos);
}

glm::vec3 	Chunk::getPos() {
	return _pos;
}

