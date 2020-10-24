#include <Render.hpp>
#include <Shader.hpp>

void 		Shader::use() const {
	glUseProgram(_shader_program);
}

void 		Shader::computeShaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];
	const char* vshader_src= "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNorm;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"out vec4 norm;\n"
		"void main()\n"
		"{\n"
			"norm = vec4(aNorm, 1.0);\n"
			"gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";
	const char *fshader_src= "#version 330 core\n"
		"in vec4 norm;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
            "if (norm.z < 0)\n"
			"   FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);\n"
            "else if (norm.z > 0)\n"
            "   FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);\n"
            "else if (norm.x < 0)\n"
            "   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
            "else if (norm.x > 0)\n"
            "   FragColor = vec4(0.4f, 0.4f, 0.4f, 1.0f);\n"
            "else if (norm.y < 0)\n"
            "   FragColor = vec4(0.9f, 0.9f, 0.9f, 1.0f);\n"
            "else if (norm.y > 0)\n"
            "   FragColor = vec4(0.3f, 0.3f, 0.3f, 1.0f);\n"
		"}\n";

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1,&vshader_src, nullptr);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cout << "VERTEX SHADER COMPILE ERROR: " << info_log << std::endl;
		Render::gameQuit();
	}
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fshader_src, nullptr);
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
	int 	loc = glGetUniformLocation(_shader_program, loc_name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);	
}
