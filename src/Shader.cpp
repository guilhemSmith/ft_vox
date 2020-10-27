#include <Render.hpp>
#include <Shader.hpp>
#include <fstream>
#include <sstream>

void 		Shader::use() const {
	glUseProgram(_shader_program);
}

void        Shader::_parseShaders(const char *vertex_shader_path, const char *frag_shader_path) {
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;


    v_shader_file.open(vertex_shader_path);
    f_shader_file.open(frag_shader_path);
    if (!v_shader_file.is_open() || !f_shader_file.is_open())
    {
        std::cout << "couln't open shaders" << std::endl;
        Render::gameQuit();
    }
    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();
    v_shader_file.close();
    f_shader_file.close();
    _vertex_shader_src = v_shader_stream.str();
    _fragment_shader_src = f_shader_stream.str();
}

void 		Shader::computeShaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];

	_parseShaders("../Shaders/vertex_shader.glsl", "../Shaders/fragment_shader.glsl");
	const char* v_shader_code = _vertex_shader_src.c_str();
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &v_shader_code, nullptr);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cout << "VERTEX SHADER COMPILE ERROR: " << info_log << std::endl;
		Render::gameQuit();
	}

	const char * f_shader_code = _fragment_shader_src.c_str();
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &f_shader_code, nullptr);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cout << "FRAGMENT SHADER COMPILE ERROR: " << info_log << std::endl;
		Render::gameQuit();
	}
	_shader_program = glCreateProgram();
	glAttachShader(_shader_program, vertex_shader);
	glAttachShader(_shader_program, fragment_shader);
	glLinkProgram(_shader_program);
	glGetProgramiv(_shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_shader_program, 512, nullptr, info_log);
		std::cout << "SHADER PROGRAM COMPILE ERROR: " << info_log << std::endl;
		Render::gameQuit();
	}
	glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void 		Shader::setMat4(const char *loc_name, glm::mat4 &mat) const {
	// int 	loc = glGetUniformLocation(_shader_program, loc_name);
	glUniformMatrix4fv(glGetUniformLocation(_shader_program, loc_name), 1, GL_FALSE, &mat[0][0]);
	// glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);	
}

void 		Shader::setTexture(const char *tex_name, int tex_id) {
	glUniform1i(glGetUniformLocation(_shader_program, tex_name), tex_id);
}
