#include "Chunk.hpp"

const unsigned int	Chunk::SIZE = 16;

Chunk::Chunk() {
	_pos = glm::vec3(0.0f, 0.0f, 0.0f);
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
