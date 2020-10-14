#include "ft_vox.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Hello world !" << std::endl;
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	TTF_Font *font = TTF_OpenFont("../fonts/Roboto-Regular.ttf", 30);

	SDL_Window *window = SDL_CreateWindow(
		"ft_vox",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		0
	);

	SDL_Surface *surface = SDL_GetWindowSurface(window);

	int img_flag = IMG_INIT_PNG;
	IMG_Init(img_flag);
	SDL_Surface *texture = IMG_Load("../textures/dirt.png");

	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
	SDL_BlitSurface(texture, NULL, surface, NULL);

	SDL_Color white = {255, 255, 255, 255};
	std::string value = "Hello world !";
	SDL_Rect position = {10, 100, 300, 50};
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, value.c_str(), white);
	SDL_BlitSurface(surfaceMessage, NULL, surface, &position);
	SDL_FreeSurface(surfaceMessage);
	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);

	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();

	return 0;
}