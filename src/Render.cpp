#include "Render.hpp"

Render::Render() {
	_win_w = 640;	
	_win_h = 480;	
}

void 	Render::drawChunks(std::vector<Chunk> &chunks) {
	glm::mat4 view = _cam.viewMat();
	_shader.setMat4("view", view);
	for (auto &chunk : chunks)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::translate(model, chunk.getPos());
		_shader.setMat4("model", model);
		chunk.draw();
	}
}

bool 	Render::gameInit() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return false;
	}
	if (TTF_Init() < 0) {
		SDL_Quit();
		return false;
	};
	_window = SDL_CreateWindow(
			"ft_vox",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			_win_w,
			_win_h,
			SDL_WINDOW_OPENGL
			// SDL_WINDOW_FULLSCREEN_DESKTOP
			);
	if (_window == NULL) {
		TTF_Quit();
		SDL_Quit();
		return false;
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
        SDL_Quit();
        return 1;
    }
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0 ,  _win_w, _win_h);
    glewExperimental = GL_TRUE; // Please expose OpenGL 3.x+ interfaces
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Glew error: " << glewGetErrorString(err) << std::endl;
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	_cam = Camera();
	_shader = Shader();
	_shader.computeShaders();
	return true;
}

void 	Render::gameLoop() {
	Time				time = Time();
	Inputs				inputs = Inputs();
	std::vector<Chunk> 	chunks;

	chunks.push_back(Chunk());
	_shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), 
			(float)_win_w / (float)_win_h, 0.1f, 100.0f);
	_shader.setMat4("projection", projection);

	while (!inputs.shouldQuit()) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		_cam.update(time.deltaTime(), inputs);

		drawChunks(chunks);

		SDL_GL_SwapWindow(_window);

		if (time.update()){
			std::cout << time.fps() << "fps" << std::endl;
		}
	}
}

void 	Render::gameQuit() {
	TTF_Quit();
	SDL_Quit();
}
