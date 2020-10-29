#include "Camera.hpp"
#include "ChunkManager.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

const glm::vec3			Camera::_WORLD_UP = {0.0f, 1.0f, 0.0f};

Camera::Camera(ChunkManager& manager): 
	_position(manager.spawnPos()),
	_direction({0, 0, -1}),
	_yaw(0.0f),
	_pitch(0.0f)
{}

void			Camera::_updateDir(const Inputs& input) {
	const glm::vec2&	mouse_rel = input.mouseRel();

	_yaw += mouse_rel.x;
	_pitch -= mouse_rel.y;
	_pitch = glm::clamp(_pitch, -89.0f, 89.0f);
	glm::vec3 dir = {
		glm::cos(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch)),
		glm::sin(glm::radians(_pitch)),
		glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch))
	};
	_direction = glm::normalize(dir);
}

void			Camera::_updatePos(float delta_time, const Inputs& input) {
	float offset = input.keyState(Inputs::_SPRINT) ? 25 * delta_time : 5 * delta_time;
	glm::vec3 input_dir = {0, 0, 0};
	
	if (input.keyState(Inputs::_FORWARD)) {
		input_dir += _direction;
	}
	if (input.keyState(Inputs::_BACKWARD)) {
		input_dir -= _direction;
	}
	if (input.keyState(Inputs::_RIGHT)) {
		input_dir += glm::normalize(glm::cross(_direction, _WORLD_UP));
	}
	if (input.keyState(Inputs::_LEFT)) {
		input_dir -= glm::normalize(glm::cross(_direction, _WORLD_UP));
	}
	if (input.keyState(Inputs::_UP)) {
		input_dir += glm::normalize(glm::cross(glm::cross(_direction, _WORLD_UP), _direction));
	}
	if (input.keyState(Inputs::_DOWN)) {
		input_dir -= glm::normalize(glm::cross(glm::cross(_direction, _WORLD_UP), _direction));
	}
	if (glm::length(input_dir) > 0) {
		input_dir = glm::normalize(input_dir);
		_position += input_dir * offset;
	}
}

void			Camera::update(float delta_time, const Inputs& input) {
	_updateDir(input);
	_updatePos(delta_time, input);
}

glm::vec3		Camera::position(void) const {
	return _position;
}

glm::vec3		Camera::direction(void) const {
	return _direction;
}

glm::mat4		Camera::viewMat(void) const {
	return glm::lookAt(_position, _position + _direction, _WORLD_UP);
}

std::ostream&	operator<<(std::ostream& os, Camera& cam) {
	glm::vec3	pos = cam.position();
	glm::vec3	dir = cam.direction();

	os << "Camera(pos[" << pos.x << ", " << pos.y << ", " <<pos.z << "], dir[" << dir.x << ", " << dir.y << ", " <<dir.z << "])";
	return os;
}