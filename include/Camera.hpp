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

	void							_updateDir(const Inputs& input);
	void							_updatePos(float delta_time, const Inputs& input);

	static const glm::vec3			_WORLD_UP;

public:
	Camera(void);

	void							update(float delta_time, const Inputs& input);

	glm::vec3						position(void) const;
	glm::vec3						direction(void) const;
	glm::mat4						viewMat(void) const;
};

std::ostream&	operator<<(std::ostream& os, Camera& cam);

#endif