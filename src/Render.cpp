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
		//chunk.remesh();
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

void 	Render::gameLoop() {
	Time				time = Time();
	Inputs				inputs = Inputs();
	_shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 
			(float)_win_w / (float)_win_h, 0.1f, 1000.0f);
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
	SDL_Quit();
	exit(0);
}
