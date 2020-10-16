#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

const glm::vec3			Camera::_WORLD_UP = {0.0f, 1.0f, 0.0f};

Camera::Camera(void): 
	_position({0, 0, 0}),
	_direction({0, 0, -1}),
	_yaw(-90),
	_pitch(0)
{}

void			Camera::_update_dir(const Inputs& input) {
	const glm::vec2&	mouse_rel = input.mouse_rel();

	_yaw += mouse_rel.x;
	_pitch -= mouse_rel.y;
	_pitch = glm::clamp(_pitch, -89.0f, 89.0f);
	glm::vec3 dir = {
		glm::cos(glm::radians(_yaw)) * glm::cos(glm::cos(_pitch)),
		glm::sin(glm::radians(_pitch)),
		glm::sin(glm::radians(_yaw)) * glm::cos(glm::radians(_pitch))
	};
	_direction = glm::normalize(dir);
}

void			Camera::_update_pos(float delta_time, const Inputs& input) {
	float offset = input.key_state(Inputs::_SPRINT) ? 20 * delta_time : delta_time;
	glm::vec3 input_dir = {0, 0, 0};
	
	if (input.key_state(Inputs::_FORWARD)) {
		input_dir += _direction;
	}
	if (input.key_state(Inputs::_BACKWARD)) {
		input_dir -= _direction;
	}
	if (input.key_state(Inputs::_LEFT)) {
		input_dir += glm::normalize(glm::cross(_direction, _WORLD_UP));
	}
	if (input.key_state(Inputs::_RIGHT)) {
		input_dir -= glm::normalize(glm::cross(_direction, _WORLD_UP));
	}
	if (glm::length(input_dir) > 0) {
		input_dir = glm::normalize(input_dir);
		_position += input_dir * offset;
	}
}

void			Camera::update(float delta_time, const Inputs& input) {
	_update_dir(input);
	_update_pos(delta_time, input);
}

glm::vec3		Camera::position(void) const {
	return _position;
}

glm::vec3		Camera::direction(void) const {
	return _direction;
}

glm::mat4		Camera::view_mat(void) const {
	return glm::lookAt(_position, _position + _direction, _WORLD_UP);
}

std::ostream&	operator<<(std::ostream& os, Camera& cam) {
	glm::vec3	pos = cam.position();
	glm::vec3	dir = cam.direction();

	os << "Camera(pos[" << pos.x << ", " << pos.y << ", " <<pos.z << "], dir[" << dir.x << ", " << dir.y << ", " <<dir.z << "])";
	return os;
}