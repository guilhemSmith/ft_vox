#include <SDL2/SDL.h>
#include "Camera.hpp"

const SDL_Scancode		Camera::_FORWARD = SDL_SCANCODE_W;
const SDL_Scancode		Camera::_BACKWARD = SDL_SCANCODE_S;
const SDL_Scancode		Camera::_LEFT = SDL_SCANCODE_A;
const SDL_Scancode		Camera::_RIGHT = SDL_SCANCODE_D;
const SDL_Scancode		Camera::_SPRINT = SDL_SCANCODE_LSHIFT;
const SDL_Scancode		Camera::_QUIT = SDL_SCANCODE_ESCAPE;

Camera::Camera(void): 
	_position(),
	_direction(),
	_should_quit(false),
	_inputs({
		{_FORWARD, false},
		{_BACKWARD, false},
		{_LEFT, false},
		{_RIGHT, false},
		{_SPRINT, false},
		{_QUIT, false}
	}),
	_handle_event({
		{SDL_WINDOWEVENT, &Camera::_poll_window_event},
		{SDL_MOUSEMOTION, &Camera::_poll_mousemotion_event},
		{SDL_KEYDOWN, &Camera::_poll_keydown_event},
		{SDL_KEYUP, &Camera::_poll_keyup_event},
	})
{}

void 			Camera::_poll_inputs(void) {
	SDL_Event		event;

	while (SDL_PollEvent(&event)) {
		try {
			(this->*_handle_event.at(event.type))(event);
		}
		catch (std::out_of_range oor) {
			// TODO handle unused event
		}
	}
	if (_inputs[Camera::_QUIT]) {
		_should_quit = true;
	}
}

void			Camera::_poll_window_event(SDL_Event& event) {
	if (event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE) {
		_should_quit = true;
	}
}

void			Camera::_poll_mousemotion_event(SDL_Event& event) {
}

void			Camera::_poll_keydown_event(SDL_Event& event) {
	_poll_key_event(event, true);
}

void			Camera::_poll_keyup_event(SDL_Event& event) {
	_poll_key_event(event, false);
}

void			Camera::_poll_key_event(SDL_Event& event, bool is_down) {
	try {
		_inputs.at(event.key.keysym.scancode) = is_down;
	}
	catch (std::out_of_range oor) {
		// TODO handle unused key
	}
}

void			Camera::update(unsigned int delta_time) {
	_poll_inputs();
}

glm::vec3		Camera::position(void) {
	return _position;
}

glm::vec3		Camera::direction(void) {
	return _direction;
}

bool			Camera::should_quit(void) {
	return _should_quit;
}

std::ostream&	operator<<(std::ostream& os, Camera& cam) {
	glm::vec3	pos = cam.position();
	glm::vec3	dir = cam.direction();
	bool		quit = cam.should_quit();

	os << "Camera(pos[" << pos.x << ", " << pos.y << ", " <<pos.z << "], dir[" << dir.x << ", " << dir.y << ", " <<dir.z << "], " << quit << ")";
	return os;
}