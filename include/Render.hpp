#ifndef RENDER_HPP
#define RENDER_HPP

#include "ft_vox.hpp"
#include "Shader.hpp"
#include "Chunk.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Inputs.hpp"
#include "Skybox.hpp"
#include "ChunkManager.hpp"

class Render {
	private:
		SDL_Window 					*_window;
		SDL_GLContext				_context;
		std::vector<SDL_Surface *> 	_textures;
		unsigned int 				_win_w;
		unsigned int 				_win_h;
		Shader 						_shader;
		Camera						_cam;
		void                        _loadCubeTextures(const char *);
		void                        _loadSkyboxTextures();
		void 						_setupCubeTextures();
		void 						_initSkybox(glm::mat4 &);
    	ChunkManager				_manager;
		std::vector<GLuint>			_texture_ids;
		unsigned int 				_skybox_id;
		Skybox 						_skybox;

	public:
		Render(unsigned int seed);
		void 	gameInit();
		void 	gameLoop();
		static void 	gameQuit();
		void 	drawChunks(std::vector<std::weak_ptr<Chunk>>&);
};

#endif
