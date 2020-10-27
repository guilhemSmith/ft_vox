#ifndef NOISE_HPP
# define NOISE_HPP

# include <vector>

class Noise {
private:
	std::vector<double>			_values;

	static const unsigned int	_SCALE;

	double						_rand(unsigned int i) const;

	double						_noise2d(unsigned int x, unsigned int y) const;
	double						_noise3d(unsigned int x, unsigned int y, unsigned int z) const;

	static double				_interpolateLinear(double a, double b, double t);
	static double				_interpolateCosine(double a, double b, double t);
	static double				_interpolateCubic(double before_a, double a, double b, double after_b, double t);

public:
	Noise(unsigned int size);

	double						noise2dSmoothLinear(double x, double y) const;
	double						noise2dSmoothCosine(double x, double y) const;

	double						noise3dSmoothLinear(double x, double y, double z) const;
	double						noise3dSmoothCosine(double x, double y, double z) const;

	double						perlin2d(int octaves, double frequency, double persistence, double x, double y) const;
	double						perlin3d(int octaves, double frequency, double persistence, double x, double y, double z) const;
};

#endif