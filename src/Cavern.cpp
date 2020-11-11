#include "Cavern.hpp"
#include "Chunk.hpp"

const unsigned int	Cavern::SIZE = 64;
const float			Cavern::STEP = 0.5;
const float			Cavern::HOLE_SIZE = 3;
const unsigned int	Cavern::DISTANCE_CHUNK_MAX = 2;

Cavern::Cavern(Noise& noise, glm::u32vec2 chunk_pos, glm::vec3 pos, float w): _chunk_pos(chunk_pos), _pos(pos), _holes() {
	float			chunk_size = static_cast<float>(Chunk::SIZE);
	glm::u32vec3	start_chunk = pos / chunk_size;
	float yaw = 0.0f;
	float pitch = -90.0f;
	
	_holes.emplace_back(pos);
	for (float s = 0; s < SIZE; s += STEP) {
		double amplitude = noise.noise1dSmoothLinear(w * SIZE + s);
		if (amplitude < 0) {
			if (amplitude < -0.5) {
				yaw += Noise::interpolateLinear(0.0, -30.0, (-amplitude - 0.5) / 0.5);
				pitch += Noise::interpolateLinear(30.0, 0.0, (-amplitude - 0.5) / 0.5);
			}
			else {
				yaw += Noise::interpolateLinear(0.0, 30.0, 1 + amplitude / 0.5);
				pitch += Noise::interpolateLinear(30.0, 0.0, 1 + amplitude / 0.5);
			}
		}
		else {
			if (amplitude > 0.5) {
				yaw += Noise::interpolateLinear(0.0, -30.0, (amplitude - 0.5) / 0.5);
				pitch += Noise::interpolateLinear(-30.0, 0.0, (amplitude - 0.5) / 0.5);
			}
			else {
				yaw += Noise::interpolateLinear(0.0, 30.0, 1 - amplitude / 0.5);
				pitch += Noise::interpolateLinear(-30.0, 0.0, 1 - amplitude / 0.5);
			}
		}
		pitch = glm::clamp(pitch, -20.0f, -160.0f);
		glm::vec3 dir = {
			glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
			glm::sin(glm::radians(pitch)),
			glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))
		};
		glm::u32vec3 hole_chunk = pos / chunk_size;
		glm::vec3 dist = glm::abs(hole_chunk - start_chunk);
		if (dist.x + HOLE_SIZE / chunk_size > DISTANCE_CHUNK_MAX
			|| dist.z + HOLE_SIZE / chunk_size > DISTANCE_CHUNK_MAX) {
			break;
		}
		pos = pos + glm::normalize(dir) * (HOLE_SIZE / 2);
		_holes.emplace_back(pos);
	}
}

const glm::u32vec2&	Cavern::chunkPos(void) const {
	return _chunk_pos;
}

void			Cavern::selectHoles(float x, float z, std::vector<glm::vec3>& selected) const {
	for (auto &pos : _holes) {
		if (pos.x + Cavern::HOLE_SIZE >= x * Chunk::SIZE && pos.x - Cavern::HOLE_SIZE <= (x + 1) * Chunk::SIZE
			&& pos.z + Cavern::HOLE_SIZE >= z * Chunk::SIZE && pos.z - Cavern::HOLE_SIZE <= (z + 1) * Chunk::SIZE) {
			selected.emplace_back(pos);
		}
	}
}