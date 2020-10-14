#include "../include/Chunk.hpp"

void 		Chunk::render() {
	_mesh.render();
}

void 		Chunk::remesh() {
	Mesh 		new_mesh;

	_mesh = new_mesh;
}
