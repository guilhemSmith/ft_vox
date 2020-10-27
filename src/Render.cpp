#include "Render.hpp"

Render::Render() {
	_win_w = 1080;
	_win_h = 720;
}

void 	Render::drawChunks(std::vector<Chunk*>& chunks) {
	glm::mat4 view = _cam.viewMat();
	_shader.setMat4("view", view);
	for (auto chunk : chunks)
	{
		if (chunk->is_empty)
			continue;
		glm::mat4 model = glm::mat4(1.0f);
		glm::translate(model, chunk->getPos());
		_shader.setMat4("model", model);
		chunk->draw();
	}
}

void 	Render::gameInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "failed to init sdl2" << std::endl;
        exit(0);
	}
	if (TTF_Init() < 0) {
		SDL_Quit();
        std::cout << "failed to init ttf" << std::endl;
        exit(0);
	};
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    int init = IMG_Init(img_flags);
    if ((init & img_flags) != img_flags) {
        std::cout << "couln't init IMG" << std::endl;
        SDL_Quit();
        TTF_Quit();
        exit(0);
    }
	_window = SDL_CreateWindow(
			"ft_vox",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			_win_w,
			_win_h,
			SDL_WINDOW_OPENGL
			// | SDL_WINDOW_FULLSCREEN_DESKTOP
			);
	if (_window == NULL) {
        std::cout << "failed to create window" << std::endl;
	    gameQuit();
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(
            SDL_GL_CONTEXT_PROFILE_MASK,
            SDL_GL_CONTEXT_PROFILE_CORE);
	_context = SDL_GL_CreateContext(_window);
    if (!_context) {
        std::cout << "failed to create gl context" << std::endl;
        SDL_DestroyWindow(_window);
        gameQuit();
    }
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0 ,  _win_w, _win_h);
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Glew error: " << glewGetErrorString(err) << std::endl;
	    gameQuit();
	}
	_shader.computeShaders();
	_skybox.computeShaders();
	

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void 	Render::_loadSkyboxTextures() {
	std::vector<std::string> textures;
	textures.push_back("../textures/skybox/right.png");
	textures.push_back("../textures/skybox/left.png");
	textures.push_back("../textures/skybox/top.png");
	textures.push_back("../textures/skybox/bot.png");
	textures.push_back("../textures/skybox/front.png");
	textures.push_back("../textures/skybox/back.png");
	glGenTextures(1, &_skybox_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_id);

	for (int i = 0; i < textures.size(); i++)
	{
		SDL_Surface *surface = IMG_Load(textures[i].c_str());
		if (!surface) {
        	std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
			gameQuit();
		}
		int Mode = GL_RGB;
    	if(surface->format->BytesPerPixel == 4) {
        	Mode = GL_RGBA;
    	}
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE0);
}

void    Render::_loadCubeTextures(const char *file) {
    SDL_Surface *surface = IMG_Load(file);
    if (!surface) {
        std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
		gameQuit();
	}

    GLuint TextureID;
    glGenTextures(1, &TextureID);
	_texture_ids.push_back(TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int Mode = GL_RGB;
    if(surface->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SDL_FreeSurface(surface);
}

void 	Render::_setupCubeTextures() {
	_loadCubeTextures("../textures/grass.png"); //will be grass top later
	_loadCubeTextures("../textures/grass_block_side.png");
	_loadCubeTextures("../textures/dirt.png");
	_loadCubeTextures("../textures/cobblestone.png"); //rock
	_loadCubeTextures("../textures/sand.png");
	_shader.setTexture("grass", 0);
	_shader.setTexture("grass_side", 1);
	_shader.setTexture("dirt", 2);
	_shader.setTexture("rock", 3);
	_shader.setTexture("sand", 4);
	for (int i = 0; i < _texture_ids.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _texture_ids[i]);
	}
}

void 	Render::_initSkybox(glm::mat4 &projection) {
	_skybox.setupBuffers();
	_skybox.use();
	_skybox.setMat4("projection", projection);
	_loadSkyboxTextures();
	_skybox.setTexture("skybox", 0);
}

void 	Render::gameLoop() {
	Time				time = Time();
	Inputs				inputs = Inputs();

	_shader.use();
	_setupCubeTextures();
	glm::mat4 projection = glm::perspective(glm::radians(80.0f),
			(float)_win_w / (float)_win_h, 0.3f, 165.0f);
	_shader.setMat4("projection", projection);
	_initSkybox(projection);	

	while (!inputs.shouldQuit()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		_cam.update(time.deltaTime(), inputs);

		_shader.use();
		std::vector<Chunk*>& chunks = _manager.getChunksFromPos(_cam.position(), _cam.direction());
		drawChunks(chunks);

		glDepthFunc(GL_LEQUAL);
		_skybox.use();
		glm::mat4 view = glm::mat4(glm::mat3(_cam.viewMat()));
		_skybox.setMat4("view", view);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_id);
		_skybox.draw();
		glDepthFunc(GL_LESS);

		SDL_GL_SwapWindow(_window);

		if (time.update()){
			std::cout << time.fps() << "fps; " << chunks.size() << " chunks; " << _cam << std::endl;
		}
	}
	_shader.delete_prog();
}

void 	Render::gameQuit() {
	TTF_Quit();
    IMG_Quit();
	SDL_Quit();
	exit(0);
}
