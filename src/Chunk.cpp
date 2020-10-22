#include "Chunk.hpp"

Chunk::Chunk(glm::vec3 pos) : _pos(pos){
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				_cubes[x][y][z] = 'a';
			}
		}
	}
	is_empty = false;
//	_cubes[0][0][0] = 'a';
//    _cubes[2][0][0] = 'a';
//    _cubes[4][0][0] = 'a';
//    _cubes[15][0][0] = 'a';
//    _cubes[15][15][15] = 'a';
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

