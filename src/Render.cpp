#include "OS.hpp"
#include "Render.hpp"

Render::Render(unsigned int seed): _manager(seed), _cam(_manager) {
	_win_w = 1080;
	_win_h = 720;
}

Render::~Render() {
	Mesh::clearBufferPool();
}

void 	Render::drawChunks(std::vector<std::weak_ptr<Chunk>>& chunks) {
	glm::mat4 view = _cam.viewMat();
	_shader.setMat4("view", view);
	for (auto chunk_ptr : chunks)
	{
	    if (auto chunk = chunk_ptr.lock()) {
            if (chunk->is_empty)
                continue;
            glm::mat4 model = glm::mat4(1.0f);
            glm::translate(model, chunk->getPos());
            _shader.setMat4("model", model);
            chunk->draw();
	    }
	}
}

void 	Render::gameInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "failed to init sdl2" << std::endl;
        exit(0);
	}
    int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
    int init = IMG_Init(img_flags);
    if ((init & img_flags) != img_flags) {
        std::cout << "couln't init IMG" << std::endl;
        SDL_Quit();
        exit(0);
    }
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	_win_w = mode.w;
	_win_h = mode.h;
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	// SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	_window = SDL_CreateWindow(
			"ft_vox",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			_win_w,
			_win_h,
			SDL_WINDOW_OPENGL
			| SDL_WINDOW_FULLSCREEN_DESKTOP
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
	// SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
	// glEnable(GL_MULTISAMPLE);
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
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void 	Render::_loadSkyboxTextures() {
	std::vector<std::string> textures;
	std::string					src = getRessourceDir();

	textures.push_back((src + "/textures/skybox/right.png").c_str());
	textures.push_back((src + "/textures/skybox/left.png").c_str());
	textures.push_back((src + "/textures/skybox/top.png").c_str());
	textures.push_back((src + "/textures/skybox/bot.png").c_str());
	textures.push_back((src + "/textures/skybox/front.png").c_str());
	textures.push_back((src + "/textures/skybox/back.png").c_str());
	glGenTextures(1, &_skybox_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_id);

	for (int i = 0; i < textures.size(); i++)
	{
		SDL_Surface *surface = IMG_Load(textures[i].c_str());
		if (!surface) {
        	std::cout << "Failed to load skybox texture: " << std::endl << textures[i] \
				<< std::endl << IMG_GetError() << std::endl;
			gameQuit();
		}
		int mode = GL_RGB;
    	if(surface->format->BytesPerPixel == 4) {
        	mode = GL_RGBA;
    	}
    	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
		SDL_FreeSurface(surface);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox_id);
}

void    Render::_loadCubeTextures(const char *file) {
    SDL_Surface *surface = IMG_Load(file);
    if (!surface) {
        std::cout << "Failed to load cube texture: " << std::endl << file << std::endl \
			<< IMG_GetError() << std::endl;
		gameQuit();
	}

    GLuint TextureID;
    glGenTextures(1, &TextureID);
	_texture_ids.push_back(TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int mode = GL_RGB;
    if(surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SDL_FreeSurface(surface);
}

void 	Render::_setupCubeTextures() {
	std::string					src = getRessourceDir();
	
	_loadCubeTextures((src + "/textures/grass.png").c_str());
	_loadCubeTextures((src + "/textures/grass_block_side.png").c_str());
	_loadCubeTextures((src + "/textures/dirt.png").c_str());
	_loadCubeTextures((src + "/textures/cobblestone.png").c_str());
	_loadCubeTextures((src + "/textures/sand.png").c_str());
	_loadCubeTextures((src + "/textures/snow.png").c_str());
	_loadCubeTextures((src + "/textures/snow_block_side.png").c_str());
	_loadCubeTextures((src + "/textures/stone.png").c_str());
	_shader.setTexture("grass", 0);
	_shader.setTexture("grass_side", 1);
	_shader.setTexture("dirt", 2);
	_shader.setTexture("rock", 3);
	_shader.setTexture("sand", 4);
	_shader.setTexture("snow", 5);
	_shader.setTexture("snow_side", 6);
	_shader.setTexture("stone", 7);
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

void    Render::_initText(Text &text) {
    glm::mat4 text_proj = glm::ortho(0.0f, static_cast<float>(_win_w), 0.0f, static_cast<float>(_win_h));
    text.computeShaders();
    text.use();
    text.setupBuffers();
    text.setMat4("projection", text_proj);
}

void	Render::_switchScreenMode(Text &text) {
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);

	if (mode.w == _win_w && mode.h == _win_h) {
		_win_w = 1080;
		_win_h = 720;

		SDL_SetWindowFullscreen(_window, 0);
		SDL_SetWindowSize(_window, _win_w, _win_h);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	else {
		_win_w = mode.w;
		_win_h = mode.h;

		SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_SetWindowSize(_window, _win_w, _win_h);
	}
	glViewport(0, 0 ,  _win_w, _win_h);
	_initText(text);
}

void 	Render::gameLoop() {
	Time				time = Time();
	Inputs				inputs = Inputs();
	Text                text(30);

	_shader.use();
	_setupCubeTextures();
	glm::mat4 projection = glm::perspective(glm::radians(80.0f),
			(float)_win_w / (float)_win_h, 0.3f, 600.0f);
	_shader.setMat4("projection", projection);
	_initSkybox(projection);	
	_manager.loadInitialChunks(_cam.position());
	_initText(text);

    std::string     fps;
	bool			fullscreen_prev = false;
	std::string		dots = ".";
	while (!inputs.shouldQuit() && _manager.isLoading()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		bool	fullscreen_curr = inputs.keyState(Inputs::_FULLSCREEN);
		if (fullscreen_prev && !fullscreen_curr) {
			_switchScreenMode(text);
		}
		fullscreen_prev = fullscreen_curr;
        text.draw("Loading" + dots, 25.0f, _win_h - 50.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        if (time.update()){
			if (dots.length() < 3) {
				dots += ".";
			}
			else {
				dots = ".";
			}
            fps = time.fps();
		}
        SDL_GL_SwapWindow(_window);
	}
	while (!inputs.shouldQuit()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		_cam.update(time.deltaTime(), inputs);
		bool	fullscreen_curr = inputs.keyState(Inputs::_FULLSCREEN);
		if (fullscreen_prev && !fullscreen_curr) {
			_switchScreenMode(text);
		}
		fullscreen_prev = fullscreen_curr;

		//draw cubes
		_shader.use();
        glBindTexture(GL_TEXTURE_2D, _texture_ids[0]);
		std::vector<std::weak_ptr<Chunk>>& chunks = _manager.getChunksFromPos(_cam.position(), _cam.direction());
		drawChunks(chunks);

		//draw skybox
		glDepthFunc(GL_LEQUAL);
		_skybox.use();
		glm::mat4 view = glm::mat4(glm::mat3(_cam.viewMat()));
		_skybox.setMat4("view", view);
		_skybox.draw();
		glDepthFunc(GL_LESS);

		//draw text
        text.draw("fps: " + fps, 25.0f, _win_h - 50.0f, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
        if (time.update()){
            fps = time.fps();
		}
        SDL_GL_SwapWindow(_window);
	}
	glDeleteTextures(_texture_ids.size(), _texture_ids.data());
	glDeleteTextures(1, &_skybox_id);
	_shader.delete_prog();
}

void 	Render::gameQuit() {
    IMG_Quit();
	SDL_Quit();
	exit(0);
}
