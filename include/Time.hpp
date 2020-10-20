#ifndef TIME_HPP
# define TIME_HPP

class Time
{
private:
	unsigned int	_delta_time;
	unsigned int	_last_frame;
	unsigned int	_frame;
	unsigned int	_frames_count;
	unsigned int	_fps;
	unsigned int	_delta_sum;

public:
	Time(void);

	bool			update(void);

	float			deltaTime(void);
	unsigned int	fps(void);
};

#endif