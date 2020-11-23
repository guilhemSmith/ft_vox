#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include <glm/glm.hpp>

const unsigned int			Chunk::SIZE = 32;

Chunk::Chunk(const World& world, glm::vec3 pos) : _pos(pos) {
	is_empty = world.fillChunk(_cubes, pos);
	is_meshed = false;
}

bool 		Chunk::hasVoxelAt(int x, int y, int z) const {
	return _cubes[x][y][z] != Voxel::Empty;
}


void        Chunk::deleteVoxel(glm::vec3 pos) {
    _cubes[pos.x][pos.y][pos.z] = Voxel::Empty;
}

Chunk::~Chunk() {
	if (is_meshed) {
		_mesh.clearBuffers();
	}
}

void 		Chunk::draw() {
	_mesh.draw();
}

void 		Chunk::remesh(std::array<std::shared_ptr<Chunk>, 6> &neighbors) {
	if (is_meshed) {
		_mesh.clearBuffers();
	}
	_mesh.init(_cubes, _pos, neighbors);
	is_meshed = true;
}

glm::vec3 	Chunk::getPos() {
	return _pos;
}

glm::u32vec3 	Chunk::getPosChunk() {
	return _pos / static_cast<float>(SIZE);
}

