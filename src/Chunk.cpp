#include "Chunk.hpp"
#include <glm/glm.hpp>

const unsigned int	Chunk::SIZE = 16;

Chunk::Chunk(const Noise& heights, glm::u32vec3 pos): _cubes(SIZE * SIZE * SIZE, 0), _mesh(), _pos(pos) {
	double chunk_height = pos.y + SIZE;
	for (int z = 0; z < SIZE; z++) {
		for (int x = 0; x < SIZE; x++) {
			double cell_height = glm::min(heights.perlin2d(4, 1.0, 0.5, (x + pos.x) / 64.0, (z + pos.z) / 64.0) * SIZE, chunk_height);
			for (int y = pos.y; y <= cell_height; y++) {
				_cubes[x + y * SIZE + z * SIZE * SIZE] = 1;
			}
		}
	}
	remesh();
}

void 		Chunk::draw() {
	_mesh.draw();
}

void 		Chunk::remesh() {
	//TODO init mesh with voxel data
	//_mesh = Mesh(_cubes);
}

const glm::vec3 	Chunk::getPos() {
	return _pos;
}
