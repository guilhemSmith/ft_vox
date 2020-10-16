#ifndef CAMERA_HPP
# define CAMERA_HPP

# include <glm/glm.hpp>
# include <ostream>
# include <map>
# include "Inputs.hpp"

class Camera {
private:
	glm::vec3						_position;
	glm::vec3						_direction;
	float							_yaw;
	float							_pitch;

	void							_update_dir(const Inputs& input);
	void							_update_pos(float delta_time, const Inputs& input);

	static const glm::vec3			_WORLD_UP;

public:
	Camera(void);

	void							update(float delta_time, const Inputs& input);

	glm::vec3						position(void) const;
	glm::vec3						direction(void) const;
	glm::mat4						view_mat(void) const;
};

std::ostream&	operator<<(std::ostream& os, Camera& cam);

#endif