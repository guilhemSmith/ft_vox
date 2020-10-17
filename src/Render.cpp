#include "Render.hpp"
#include "Camera.hpp"
#include "Time.hpp"
#include "Inputs.hpp"

Render::Render() {
	_win_w = 640;	
	_win_h = 480;	
}

void 	Render::compute_shaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];
	const char* vshader_src= "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec4 pos;\n"
		"void main()\n"
		"{\n"
			"pos = vec4(aPos, 1.0);\n"
			"gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char *fshader_src= "#version 330 core\n"
		"in vec4 pos;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(pos);\n"
		"}\n";

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1,&vshader_src, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		std::cout << "VERTEX SHADER COMPILE ERROR: " << info_log << std::endl;
		gameQuit();
	}
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fshader_src, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "FRAGMENT SHADER COMPILE ERROR: " << info_log << std::endl;
		gameQuit();
	}
	_shader_program = glCreateProgram();
	glAttachShader(_shader_program, vertex_shader);
	glAttachShader(_shader_program, fragment_shader);
	glLinkProgram(_shader_program);
	glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_shader_program, 512, NULL, info_log);
		std::cout << "SHADER PROGRAM COMPILE ERROR: " << info_log << std::endl;
		gameQuit();
	}
	glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void 	Render::drawMeshes(std::vector<Mesh> &meshes) {
	for (auto &mesh : meshes)
	{
		mesh.draw();
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	_context = SDL_GL_CreateContext(_window);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glViewport(0, 0 ,  _win_w, _win_h);
	GLenum err = glewInit();
	return true;
}

void 	Render::gameLoop() {
	Camera			cam = Camera();
	Time			time = Time();
	Inputs			inputs = Inputs();
	std::vector<Mesh> 	meshes;

	meshes.emplace_back(Mesh());

	compute_shaders();
	glUseProgram(_shader_program);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)_win_w / (float)_win_h, 0.1f, 100.0f);
	int projLoc = glGetUniformLocation(_shader_program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

	//TODO render-> drawCHUNKS -> draw meshes
	//
	//TODO translate model mat with chunck position
	glm::mat4 model = glm::mat4(1.0f);
	int modelLoc = glGetUniformLocation(_shader_program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);

	while (!inputs.should_quit()) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		inputs.update();
		cam.update(time.delta_time(), inputs);
		int viewloc = glGetUniformLocation(_shader_program, "view");
		glm::mat4 view = cam.view_mat();
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, &view[0][0]);
		drawMeshes(meshes);

		SDL_GL_SwapWindow(_window);

		if (time.update()){
			std::cout << time.fps() << "fps; " << cam << std::endl;
		}
	}
}

void 	Render::gameQuit() {
	TTF_Quit();
	SDL_Quit();
}
