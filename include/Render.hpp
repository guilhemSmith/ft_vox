#ifndef RENDER_HPP
#define RENDER_HPP

#include "ft_vox.hpp"
#include "Shader.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Inputs.hpp"
#include "World.hpp"

class Render {
	private:
		SDL_Window 					*_window;
		SDL_GLContext				_context;
		std::vector<SDL_Surface *> 	_textures;
		unsigned int 				_win_w;
		unsigned int 				_win_h;
		Shader 						_shader;
		Camera						_cam;
		void                        _loadTextures(const char *);
		void 						_setupTextures();
		World						_world;
		std::vector<GLuint>			_texture_ids;

	public:
		Render();
		void 	gameInit();
		void 	gameLoop();
		static void 	gameQuit();
		void 	drawChunks(std::vector<Chunk*>&);
};

#endif
