#include "Time.hpp"
#include <SDL2/SDL.h>

Time::Time(void): _delta_time(0), _last_frame(0), _frame(0), _frames_count(0), _fps(0), _delta_sum(0) {}

bool			Time::update(void) {
	_frame = SDL_GetTicks();
	_delta_time = _frame - _last_frame;
	_last_frame = _frame;

	if (_delta_sum + _delta_time > 1000) {
		_fps = _frames_count;
		_frames_count = 1;
		_delta_sum = _delta_sum + _delta_time - 1000;
		return true;
	}
	else {
		_frames_count++;
		_delta_sum += _delta_time;
		return false;
	}
}

float			Time::delta_time(void) {
	float seconds = static_cast<float>(_delta_time) / 1000.0;
	return seconds;
}

unsigned int	Time::fps(void) {
	return _fps;
}