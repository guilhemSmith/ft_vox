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
    int img_flags = IMG_INIT_PNG;
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
	_cam = Camera();
	_shader = Shader();
	_shader.computeShaders();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void 	Redner::_loadSkybox() {
	unsigned int skybox;
	std::vector<string> textures;
	textures.push_back("right.jpg");
	textures.push_back("left.jpg");
	textures.push_back("top.jpg");
	textures.push_back("bottom.jpg");
	textures.push_back("front.jpg");
	textures.push_back("back.jpg");
	glGenTextures(1, &skybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);

	for (auto &t: textures))
	{
		SDL_Surface *surface = IMG_Load(t);
		if (!surface) {
        	std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
			gameQuit();
		}
	}

}

void    Render::_loadTextures(const char *file) {
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

void 	Render::_setupTextures() {
	_loadTextures("../textures/melon_top.png"); //will be grass top later
	_loadTextures("../textures/grass_block_side.png");
	_loadTextures("../textures/dirt.png");
	_loadTextures("../textures/cobblestone.png"); //rock
	_loadTextures("../textures/sand.png");
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

void 	Render::gameLoop() {
	Time				time = Time();
	Inputs				inputs = Inputs();

	_shader.use();
	_setupTextures();
	glm::mat4 projection = glm::perspective(glm::radians(80.0f),
			(float)_win_w / (float)_win_h, 0.1f, 160.0f);
	_shader.setMat4("projection", projection);

	
	while (!inputs.shouldQuit()) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		_cam.update(time.deltaTime(), inputs);

		std::vector<Chunk*>& chunks = _world.getChunksFromPos(_cam.position(), _cam.direction());
		drawChunks(chunks);

		SDL_GL_SwapWindow(_window);

		if (time.update()){
			std::cout << time.fps() << "fps; " << chunks.size() << " chunks; " << _cam << std::endl;
		}
	}
}

void 	Render::gameQuit() {
	TTF_Quit();
    IMG_Quit();
	SDL_Quit();
	exit(0);
}
