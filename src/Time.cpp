#include "Time.hpp"
#include <SDL.h>

Time::Time(void): _delta_time(0), _last_frame(0), _frame(0), _frames_count(0), _fps(0), _delta_sum(0) {}

bool			Time::update(void) {
	_frame = SDL_GetTicks();
	_delta_time = _frame - _last_frame;
	_last_frame = _frame;

	_delta_sum += _delta_time;
	if (_delta_sum >= 1000) {
		_fps = _frames_count;
		_frames_count = 1;
		_delta_sum -= 1000;
		return true;
	}
	else {
		_frames_count++;
		return false;
	}
}

float			Time::deltaTime(void) {
	float seconds = static_cast<float>(_delta_time) / 1000.0;
	return seconds;
}

std::string	Time::fps(void) {
	return std::to_string(_fps);
}