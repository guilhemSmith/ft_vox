#include "ft_vox.hpp"
#include "Render.hpp"
#include "OS.hpp"
#include <time.h>
#include <stdlib.h>

unsigned int		parseArg(int argc, char *argv[]) {
	unsigned int	seed = time(NULL);
	if (argc > 1) {
		seed = std::atoi(argv[1]);
	}
	std::cout << "Seed used: " << seed << std::endl;
	srand(seed);
	return seed;
}

int main(int argc, char *argv[])
{
	Render 		render(parseArg(argc, argv));

	render.gameInit();
	render.gameLoop();
	std::cout << "Game Over." << std::endl;
    render.gameQuit();
}
