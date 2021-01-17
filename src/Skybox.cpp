#include "OS.hpp"
#include "Skybox.hpp"
#include "Render.hpp"
#include "Resources.hpp"
#include <fstream>
#include <sstream>

Skybox::Skybox() {
    _vertices.emplace_back(-1.0f , 1.0f , -1.0f);
    _vertices.emplace_back(-1.0f , -1.0f , -1.0f);
    _vertices.emplace_back(1.0f , -1.0f , -1.0f);
    _vertices.emplace_back(1.0f , 1.0f , -1.0f);
    _vertices.emplace_back(-1.0f , -1.0f , 1.0f);
    _vertices.emplace_back(-1.0f , 1.0f , 1.0f);
    _vertices.emplace_back(1.0f , -1.0f , 1.0f);
    _vertices.emplace_back(1.0f , 1.0f , 1.0f);
    _indices.push_back(0);
    _indices.push_back(1);
    _indices.push_back(2);
    _indices.push_back(2);
    _indices.push_back(3);
    _indices.push_back(0);
    _indices.push_back(4);
    _indices.push_back(1);
    _indices.push_back(0);
    _indices.push_back(0);
    _indices.push_back(5);
    _indices.push_back(4);
    _indices.push_back(2);
    _indices.push_back(6);
    _indices.push_back(7);
    _indices.push_back(7);
    _indices.push_back(3);
    _indices.push_back(2);
    _indices.push_back(4);
    _indices.push_back(5);
    _indices.push_back(7);
    _indices.push_back(7);
    _indices.push_back(6);
    _indices.push_back(4);
    _indices.push_back(0);
    _indices.push_back(3);
    _indices.push_back(7);
    _indices.push_back(7);
    _indices.push_back(5);
    _indices.push_back(0);
    _indices.push_back(1);
    _indices.push_back(4);
    _indices.push_back(2);
    _indices.push_back(2);
    _indices.push_back(4);
    _indices.push_back(6);
}


void        Skybox::setupBuffers() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)nullptr);
    glBindVertexArray(0);
}

void 		Skybox::use() const {
	glUseProgram(_shader_program);
}

void        Skybox::_parseShaders(const char *vertex_shader_path, const char *frag_shader_path) {
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;


    v_shader_file.open(vertex_shader_path);
    f_shader_file.open(frag_shader_path);
    if (!v_shader_file.is_open() || !f_shader_file.is_open())
    {
        std::cout << "couln't open skybox shaders" << std::endl << vertex_shader_path << std::endl \
			<< frag_shader_path << std::endl;
        Render::gameQuit();
    }
    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();
    v_shader_file.close();
    f_shader_file.close();
    _vertex_shader_src = v_shader_stream.str();
    _fragment_shader_src = f_shader_stream.str();
}

void 		Skybox::computeShaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];
    std::string vertex_shader_path = Resources::getPath("Shaders/skybox_vertex_shader.glsl");
    std::string fragment_shader_path = Resources::getPath("Shaders/skybox_fragment_shader.glsl");

	_parseShaders(vertex_shader_path.c_str(), fragment_shader_path.c_str());
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

void 		Skybox::setTexture(const char *tex_name, int tex_id) {
	glUniform1i(glGetUniformLocation(_shader_program, tex_name), tex_id);
}

void 		Skybox::setMat4(const char *loc_name, glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(_shader_program, loc_name), 1, GL_FALSE, &mat[0][0]);
}

void        Skybox::draw() {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
    glDeleteProgram(_shader_program);
}