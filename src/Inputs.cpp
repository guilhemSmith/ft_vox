#include "Inputs.hpp"

const float				Inputs::_MOUSE_SENSITIVITY = 0.1f;

const SDL_Scancode		Inputs::_FORWARD = SDL_SCANCODE_W;
const SDL_Scancode		Inputs::_BACKWARD = SDL_SCANCODE_S;
const SDL_Scancode		Inputs::_LEFT = SDL_SCANCODE_A;
const SDL_Scancode		Inputs::_RIGHT = SDL_SCANCODE_D;
const SDL_Scancode		Inputs::_SPRINT = SDL_SCANCODE_LSHIFT;
const SDL_Scancode		Inputs::_QUIT = SDL_SCANCODE_ESCAPE;

Inputs::Inputs(void):
	_should_quit(false),
	_mouse_rel(),
	_keys({
		{_FORWARD, false},
		{_BACKWARD, false},
		{_LEFT, false},
		{_RIGHT, false},
		{_SPRINT, false},
		{_QUIT, false}
	}),
	_handle_event({
		{SDL_WINDOWEVENT, &Inputs::_poll_window_event},
		{SDL_MOUSEMOTION, &Inputs::_poll_mousemotion_event},
		{SDL_KEYDOWN, &Inputs::_poll_keydown_event},
		{SDL_KEYUP, &Inputs::_poll_keyup_event},
	})
{}

void			Inputs::_poll_window_event(SDL_Event& event) {
	if (event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE) {
		_should_quit = true;
	}
}

void			Inputs::_poll_mousemotion_event(SDL_Event& event) {
	_mouse_rel.x = event.motion.xrel * _MOUSE_SENSITIVITY;
	_mouse_rel.y = event.motion.yrel * _MOUSE_SENSITIVITY;
}

void			Inputs::_poll_keydown_event(SDL_Event& event) {
	_poll_key_event(event, true);
}

void			Inputs::_poll_keyup_event(SDL_Event& event) {
	_poll_key_event(event, false);
}

void			Inputs::_poll_key_event(SDL_Event& event, bool is_down) {
	try {
		_keys.at(event.key.keysym.scancode) = is_down;
	}
	catch (std::out_of_range oor) {
		// TODO handle unused key
	}
}

void				Inputs::update(void) {
	SDL_Event		event;
	_mouse_rel.x = 0;
	_mouse_rel.y = 0;
	while (SDL_PollEvent(&event)) {
		try {
			(this->*_handle_event.at(event.type))(event);
		}
		catch (std::out_of_range oor) {
			// TODO handle unused event
		}
	}
	if (_keys[_QUIT]) {
		_should_quit = true;
	}
}

bool				Inputs::should_quit(void) const {
	return _should_quit;
}

const glm::vec2&	Inputs::mouse_rel(void) const {
	return _mouse_rel;
}

bool				Inputs::key_state(SDL_Scancode key) const {
	try {
		return _keys.at(key);
	}
	catch (std::out_of_range oor) {
		return false;
	}
}