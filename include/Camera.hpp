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
	glm::vec2						_mouse_rel;
	std::map<SDL_Scancode, bool>	_inputs;

	void							_poll_inputs(void);

	void							_poll_window_event(SDL_Event& event);
	void							_poll_mousemotion_event(SDL_Event& event);
	void							_poll_keydown_event(SDL_Event& event);
	void							_poll_keyup_event(SDL_Event& event);
	void							_poll_key_event(SDL_Event& event, bool is_down);

	typedef void	(Camera::*_polled_event)(SDL_Event&);
	const std::map<Uint32, _polled_event> _handle_event;

	static const SDL_Scancode		_FORWARD;
	static const SDL_Scancode		_BACKWARD;
	static const SDL_Scancode		_LEFT;
	static const SDL_Scancode		_RIGHT;
	static const SDL_Scancode		_SPRINT;
	static const SDL_Scancode		_QUIT;

public:
	Camera(void);

	void							update(unsigned int delta_time);

	glm::vec3						position(void);
	glm::vec3						direction(void);
	bool							should_quit(void);

};

std::ostream&	operator<<(std::ostream& os, Camera& cam);

#endif