#ifndef CAMERA_HPP
# define CAMERA_HPP

# include <glm/glm.hpp>
# include <ostream>
# include <map>

class Camera {
private:
	glm::vec3						_position;
	glm::vec3						_direction;
	bool							_should_quit;
	std::map<SDL_Scancode, bool>	_inputs;

	void							_poll_inputs(void);

	void							_poll_window_event(SDL_Event& event);
	void							_poll_mousemotion_event(SDL_Event& event);
	void							_poll_keydown_event(SDL_Event& event);
	void							_poll_keyup_event(SDL_Event& event);
	void							_poll_key_event(SDL_Event& event, bool is_down);

	static SDL_Scancode const		_FORWARD = SDL_SCANCODE_W;
	static SDL_Scancode const		_BACKWARD = SDL_SCANCODE_S;
	static SDL_Scancode const		_LEFT = SDL_SCANCODE_A;
	static SDL_Scancode const		_RIGHT = SDL_SCANCODE_D;
	static SDL_Scancode const		_SPRINT = SDL_SCANCODE_LSHIFT;

public:
	Camera(void);

	void							update(unsigned int delta_time);

	glm::vec3						position(void);
	glm::vec3						direction(void);
	bool							should_quit(void);

};

std::ostream&	operator<<(std::ostream& os, Camera& cam);

#endif