#include "Camera.hpp"
#include "ChunkManager.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

const glm::vec3			Camera::_WORLD_UP = {0.0f, 1.0f, 0.0f};

Camera::Camera(ChunkManager& manager): 
	_position(manager.spawnPos()),
	_direction({0, 0, -1}),
	_yaw(0.0f),
	_pitch(0.0f),
	_chunk_manager_ref(manager)
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
	float offset = input.keyState(Inputs::_SPRINT) ? 20 * delta_time : delta_time;
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
	if (glm::length(input_dir) > 0) {
		input_dir = glm::normalize(input_dir);
		_position += input_dir * offset;
	}

}

void 			Camera::deleteVoxel(float max_dist, const Inputs& input) {
    if (!input.mouseDown())
        return ;
    float dir_length = glm::length(_direction);

    std::cout << "RAYCAST!" << std::endl;
    if (dir_length < 0.01) {
        std::cout << "dir length too small" << std::endl;
        return ;
    }

    glm::vec3 normalized_dir = glm::normalize(_direction);

    float running_distance = 0.0f;

    int start_x = floor(_position.x);
    int start_y = floor(_position.y);
    int start_z = floor(_position.z);

    int step_x = (normalized_dir.x > 0 ) ? 1 : -1;
    int step_y = (normalized_dir.y > 0 ) ? 1 : -1;
    int step_z = (normalized_dir.z > 0 ) ? 1 : -1;

    float rd_x_delta = abs(1 / normalized_dir.x);
    float rd_y_delta = abs(1 / normalized_dir.y);
    float rd_z_delta = abs(1 / normalized_dir.z);

    float x_dist = (step_x > 0) ? (start_x + 1 - _position.x) : (_position.x - start_x);
    float y_dist = (step_y > 0) ? (start_y + 1 - _position.y) : (_position.y - start_y);
    float z_dist = (step_z > 0) ? (start_z + 1 - _position.z) : (_position.z - start_z);

//    float rd_x_max = (tx_delta < ChunkManager::SIZES_VOXELS.x) ? tx_delta * x_dist : ChunkManager::SIZES_VOXELS.x;
//    float rd_y_max = (ty_delta < ChunkManager::SIZES_VOXELS.x) ? ty_delta * y_dist : ChunkManager::SIZES_VOXELS.y;
//    float rd_z_max = (tz_delta < ChunkManager::SIZES_VOXELS.z) ? tz_delta * z_dist : ChunkManager::SIZES_VOXELS.z;
    float rd_x_max = (rd_x_delta < 40000) ? rd_x_delta * x_dist : 40000;
    float rd_y_max = (rd_y_delta < 40000) ? rd_y_delta * y_dist : 40000;
    float rd_z_max = (rd_z_delta < 40000) ? rd_z_delta * z_dist : 40000;

    while (running_distance <= max_dist)
    {
        bool   hit = _chunk_manager_ref.tryDeleteVoxel(glm::u32vec3(start_x, start_y, start_z));
        if (hit) {
            std::cout << "hit found" << std::endl;
            return;
        }

        if (rd_x_max < rd_y_max) {
            if (rd_x_max < rd_z_max) {
                start_x += step_x;
                running_distance = rd_x_max;
                rd_x_max += rd_x_delta;
            } else {
                start_z += step_z;
                running_distance = rd_z_max;
                rd_z_max += rd_z_delta;
            }
        } else {
            if (rd_y_max < rd_y_max) {
                start_y += step_y;
                running_distance = rd_y_max;
                rd_y_max += rd_y_delta;
            } else {
                start_z += step_z;
                running_distance = rd_z_max;
                rd_z_max += rd_z_delta;
            }
        }
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