#ifndef RENDER_HPP
#define RENDER_HPP

#include "ft_vox.hpp"
#include "Mesh.hpp"

class Render {
	private:
		SDL_Window 					*_window;
		SDL_GLContext				_context;
		std::vector<SDL_Surface *> 	_textures;
		unsigned int 				_shader_program;
		unsigned int 				_win_w;
		unsigned int 				_win_h;

	public:
		Render();
		void 	compute_shaders();
		bool 	gameInit();
		void 	gameLoop();
		void 	gameQuit();
		void 	drawMeshes(std::vector<Mesh> &);
};

#endif
