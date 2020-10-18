#include "ft_vox.hpp"
#include "Render.hpp"

int main(int argc, char *argv[])
{
	Render 		render;

	if (render.gameInit()) {
		render.gameLoop();

		render.gameQuit();
		std::cout << "Game Over." << std::endl;
		return 0;
	}
	else {
		std::cout << "Game initialization failed." << std::endl;
		return -1;
	}
}
