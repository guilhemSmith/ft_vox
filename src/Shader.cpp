#include "Shader.hpp"

void 		Shader::use() {
	glUseProgram(_shader_program);
}

void 		Shader::computeShaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];
	const char* vshader_src= "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		//"layout (location = 1) in vec3 aNorm;\n"
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
		//TODO better exit
		exit(0);
	}
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fshader_src, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		std::cout << "FRAGMENT SHADER COMPILE ERROR: " << info_log << std::endl;
		exit(0);
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
		exit(0);
	}
	glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void 		Shader::setMat4(const char *loc_name, glm::mat4 &mat) {
	int 	loc = glGetUniformLocation(_shader_program, loc_name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);	
}
