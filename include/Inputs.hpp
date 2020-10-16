#ifndef INPUTS_HPP
# define INPUTS_HPP

# include <glm/glm.hpp>
#include <SDL2/SDL.h>
# include <map>

class Inputs {
private:
	bool							_should_quit;
	glm::vec2						_mouse_rel;
	std::map<SDL_Scancode, bool>	_keys;

	void							_poll_window_event(SDL_Event& event);
	void							_poll_mousemotion_event(SDL_Event& event);
	void							_poll_keydown_event(SDL_Event& event);
	void							_poll_keyup_event(SDL_Event& event);
	void							_poll_key_event(SDL_Event& event, bool is_down);

	typedef void	(Inputs::*_polled_event)(SDL_Event&);
	const std::map<Uint32, _polled_event> _handle_event;

	static const float				_MOUSE_SENSITIVITY;
public:
	Inputs(void);

	void							update(void);
	bool							should_quit(void) const;
	const glm::vec2&				mouse_rel(void) const;
	bool							key_state(SDL_Scancode key) const;

	static const SDL_Scancode		_FORWARD;
	static const SDL_Scancode		_BACKWARD;
	static const SDL_Scancode		_LEFT;
	static const SDL_Scancode		_RIGHT;
	static const SDL_Scancode		_SPRINT;
	static const SDL_Scancode		_QUIT;
};

#endif