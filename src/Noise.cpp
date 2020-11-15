#include "Noise.hpp"
#include <stdlib.h>
#include <cmath>
#include <glm/glm.hpp>

const unsigned int	Noise::_SCALE = 85000;

Noise::Noise(unsigned int size): _values() {
	_values.reserve(size);
	for (auto i = 0; i < size; i++) {
		_values.push_back(static_cast <double> (rand()) / static_cast <double> (RAND_MAX / 2) - 1.0);
	}
}

double				Noise::_rand(unsigned int i) const {
	return _values[i % _values.size()];
}

double				Noise::_noise1d(unsigned int t) const {
	return _rand(t);
}

double				Noise::_noise2d(unsigned int x, unsigned int y) const {
	unsigned int tmp = _rand(x) * _SCALE;
	return _rand(tmp + y);
}

double				Noise::_noise3d(unsigned int x, unsigned int y, unsigned int z) const {
	unsigned int tmp = _rand(x) * _SCALE;
	tmp = _rand(tmp + y) * _SCALE;
	return _rand(tmp + z);
}

double				Noise::interpolateLinear(double a, double b, double t) {
	return a * (1.0 - t) + b * t;
}

double				Noise::interpolateCosine(double a, double b, double t) {
	double c = (1.0 - std::cos(t * M_PI)) / 2;
	return a * (1.0 - c) + b * c;
}

double				Noise::_interpolateCubic(double before_a, double a, double b, double after_b, double t) {
	double a3 = -0.5 * before_a + 1.5 * a - 1.5 * b + 0.5 * after_b;
    double a2 = before_a - 2.5 * a + 2 * b - 0.5 * after_b;
    double a1 = -0.5 * before_a + 0.5 * b;
    double a0 = a;

	return (a3 * t * t * t) + (a2 * t * t) + (a1 * t) + a0;
}

double				Noise::noise1dSmoothLinear(double t) const {
	int		integer_t = static_cast<int>(t);
	double	fractional_t = t - integer_t;

	double a = _noise1d(integer_t);
	double b = _noise1d(integer_t + 1);

	return interpolateLinear(a, b, fractional_t);
}

double				Noise::noise1dSmoothCosine(double t) const {
	int		integer_t = static_cast<int>(t);
	double	fractional_t = t - integer_t;

	double a = _noise1d(integer_t);
	double b = _noise1d(integer_t + 1);

	return interpolateCosine(a, b, fractional_t);
}

double				Noise::noise2dSmoothLinear(double x, double y) const {
	int		integer_x = static_cast<int>(x);
	int		integer_y = static_cast<int>(y);
	double	fractional_x = x - integer_x;
	double	fractional_y = y - integer_y;

	double a0 = _noise2d(integer_x, integer_y);
	double a1 = _noise2d(integer_x + 1, integer_y);
	double b0 = _noise2d(integer_x, integer_y + 1);
	double b1 = _noise2d(integer_x + 1, integer_y + 1);

	double a = interpolateLinear(a0, a1, fractional_x);
	double b = interpolateLinear(b0, b1, fractional_x);

	return interpolateLinear(a, b, fractional_y);
}

double				Noise::noise2dSmoothCosine(double x, double y) const {
	int		integer_x = static_cast<int>(x);
	int		integer_y = static_cast<int>(y);
	double	fractional_x = x - integer_x;
	double	fractional_y = y - integer_y;

	double a0 = _noise2d(integer_x, integer_y);
	double a1 = _noise2d(integer_x + 1, integer_y);
	double b0 = _noise2d(integer_x, integer_y + 1);
	double b1 = _noise2d(integer_x + 1, integer_y + 1);

	double a = interpolateCosine(a0, a1, fractional_x);
	double b = interpolateCosine(b0, b1, fractional_x);

	return interpolateCosine(a, b, fractional_y);
}

double				Noise::noise3dSmoothLinear(double x, double y, double z) const {
	int		integer_x = static_cast<int>(x);
	int		integer_y = static_cast<int>(y);
	int		integer_z = static_cast<int>(z);
	double	fractional_x = x - integer_x;
	double	fractional_y = y - integer_y;
	double	fractional_z = z - integer_z;

	double a0 = _noise3d(integer_x, integer_y, integer_z);
	double a1 = _noise3d(integer_x + 1, integer_y, integer_z);

	double b0 = _noise3d(integer_x, integer_y + 1, integer_z);
	double b1 = _noise3d(integer_x + 1, integer_y + 1, integer_z);
	
	double c0 = _noise3d(integer_x, integer_y, integer_z + 1);
	double c1 = _noise3d(integer_x + 1, integer_y, integer_z + 1);
	
	double d0 = _noise3d(integer_x, integer_y + 1, integer_z + 1);
	double d1 = _noise3d(integer_x + 1, integer_y + 1, integer_z + 1);

	double a = interpolateLinear(a0, a1, fractional_x);
	double b = interpolateLinear(b0, b1, fractional_x);
	double v = interpolateLinear(a, b, fractional_y);

	double c = interpolateLinear(c0, c1, fractional_x);
	double d = interpolateLinear(d0, d1, fractional_x);
	double w = interpolateLinear(c, d, fractional_y);

	return interpolateLinear(v, w, fractional_z);
}

double				Noise::noise3dSmoothCosine(double x, double y, double z) const {
	int		integer_x = static_cast<int>(x);
	int		integer_y = static_cast<int>(y);
	int		integer_z = static_cast<int>(z);
	double	fractional_x = x - integer_x;
	double	fractional_y = y - integer_y;
	double	fractional_z = z - integer_z;

	double a0 = _noise3d(integer_x, integer_y, integer_z);
	double a1 = _noise3d(integer_x + 1, integer_y, integer_z);

	double b0 = _noise3d(integer_x, integer_y + 1, integer_z);
	double b1 = _noise3d(integer_x + 1, integer_y + 1, integer_z);
	
	double c0 = _noise3d(integer_x, integer_y, integer_z + 1);
	double c1 = _noise3d(integer_x + 1, integer_y, integer_z + 1);
	
	double d0 = _noise3d(integer_x, integer_y + 1, integer_z + 1);
	double d1 = _noise3d(integer_x + 1, integer_y + 1, integer_z + 1);

	double a = interpolateCosine(a0, a1, fractional_x);
	double b = interpolateCosine(b0, b1, fractional_x);
	double v = interpolateCosine(a, b, fractional_y);

	double c = interpolateCosine(c0, c1, fractional_x);
	double d = interpolateCosine(d0, d1, fractional_x);
	double w = interpolateCosine(c, d, fractional_y);

	return interpolateCosine(v, w, fractional_z);
}

double				Noise::perlin1d(int octaves, double frequency, double persistence, double x) const {
	double r = 0.0;
	double f = frequency;
	double amplitude = 1.0;

	for (auto i = 0; i < octaves; i++) {
		int t = i * 4096;
		r += noise1dSmoothCosine(x * f + t) * amplitude;
		amplitude *= persistence;
		f *= 2;
	}

	double geo_lim = (1 - persistence) / (1 - amplitude);
	return r * geo_lim;
}

double				Noise::perlin2d(int octaves, double frequency, double persistence, double x, double y) const {
	double r = 0.0;
	double f = frequency;
	double amplitude = 1.0;

	for (auto i = 0; i < octaves; i++) {
		int t = i * 4096;
		r += noise2dSmoothCosine(x * f + t, y * f + t) * amplitude;
		amplitude *= persistence;
		f *= 2;
	}

	double geo_lim = (1 - persistence) / (1 - amplitude);
	return r * geo_lim;
}

double				Noise::perlin3d(int octaves, double frequency, double persistence, double x, double y, double z) const {
	double r = 0.0;
	double f = frequency;
	double amplitude = 1.0;

	for (auto i = 0; i < octaves; i++) {
		int t = i * 4096;
		r += noise3dSmoothCosine(x * f + t, y * f + t, z * f + t) * amplitude;
		amplitude *= persistence;
		f *= 2;
	}

	double geo_lim = (1 - persistence) / (1 - amplitude);
	return r * geo_lim;
}