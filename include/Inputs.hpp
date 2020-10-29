#ifndef INPUTS_HPP
# define INPUTS_HPP

# include <glm/glm.hpp>
# include "libs.hpp"
# include <map>

class Inputs {
private:
	bool							_should_quit;
	glm::vec2						_mouse_rel;
	std::map<SDL_Scancode, bool>	_keys;

	void							_pollWindowEvent(SDL_Event& event);
	void							_pollMousemotionEvent(SDL_Event& event);
	void							_pollKeydownEvent(SDL_Event& event);
	void							_pollKeyupEvent(SDL_Event& event);
	void							_pollKeyEvent(SDL_Event& event, bool is_down);

	typedef void	(Inputs::*_polled_event)(SDL_Event&);
	const std::map<Uint32, _polled_event> _handle_event;

	static const float				_MOUSE_SENSITIVITY;
public:
	Inputs(void);

	void							update(void);
	bool							shouldQuit(void) const;
	const glm::vec2&				mouseRel(void) const;
	bool							keyState(SDL_Scancode key) const;

	static const SDL_Scancode		_FORWARD;
	static const SDL_Scancode		_BACKWARD;
	static const SDL_Scancode		_LEFT;
	static const SDL_Scancode		_RIGHT;
	static const SDL_Scancode		_UP;
	static const SDL_Scancode		_DOWN;
	static const SDL_Scancode		_SPRINT;
	static const SDL_Scancode		_QUIT;
};

#endif