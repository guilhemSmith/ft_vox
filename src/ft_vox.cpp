#include "ft_vox.hpp"
#include "Camera.hpp"
#include "Time.hpp"

bool	game_init(SDL_Window *&window) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}
	if (TTF_Init() < 0) {
		SDL_Quit();
		return false;
	};
	window = SDL_CreateWindow(
		"ft_vox",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);
	if (window == NULL) {
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	return true;
}

void	game_loop(SDL_Window *window) {
	Camera			cam = Camera();
	Time			time = Time();
	
	SDL_Surface		*surface = SDL_GetWindowSurface(window);

	while (!cam.should_quit()) {
		cam.update(time.delta_time());
		
		SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
		SDL_UpdateWindowSurface(window);

		if (time.update()){
			std::cout << time.fps() << "fps" << std::endl;
		}
	}
}

int main(int argc, char *argv[])
{
	SDL_Window *window;

	if (game_init(window)) {
		game_loop(window);

		std::cout << "Game Over." << std::endl;
		TTF_Quit();
		SDL_Quit();
		return 0;
	}
	else {
		std::cout << "Game initialization failed." << std::endl;
		return -1;
	}
}