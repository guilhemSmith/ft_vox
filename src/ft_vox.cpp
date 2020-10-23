#include "ft_vox.hpp"
#include "Render.hpp"
#include "OS.hpp"

int main(int argc, char *argv[])
{
	Render 		render;

	render.gameInit();
	render.gameLoop();
	std::cout << "Game Over." << std::endl;
    render.gameQuit();
}
