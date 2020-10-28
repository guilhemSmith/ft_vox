#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include <glm/glm.hpp>

const unsigned int			Chunk::SIZE = 32;

Chunk::Chunk(const Noise& heights, glm::vec3 pos) : _pos(pos) {
	is_empty = true;
	for (int z = 0; z < SIZE; z++) {
		for (int x = 0; x < SIZE; x++) {
			double height = heights.perlin2d(4, 1.0, 0.5, (x + pos.x) / ChunkManager::NOISE_STRETCH, (z + pos.z) / ChunkManager::NOISE_STRETCH);
			height = height * ChunkManager::SIZES_VOXELS.y;
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
	is_meshed = false;
}

bool 		Chunk::hasVoxelAt(int x, int y, int z) const {
	return _cubes[x][y][z] != Voxel::Empty;
}

Chunk::~Chunk() {
	_mesh.clearBuffers();
}

void 		Chunk::draw() {
	_mesh.draw();
}

void 		Chunk::remesh(std::array<Chunk*, 6> &neighbors) {
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

