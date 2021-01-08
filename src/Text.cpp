#include "OS.hpp"
#include "Text.hpp"
#include "Render.hpp"
#include <fstream>
#include <sstream>
#include <ft2build.h>
#include FT_FREETYPE_H

Text::Text(unsigned int font_size) {
    FT_Library  ft;
    FT_Face     face;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE Could not init FreeType Library" << std::endl;
        Render::gameQuit();
    }
    if (FT_New_Face(ft, "../fonts/Roboto-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        Render::gameQuit();
    }
    FT_Set_Pixel_Sizes(face, 0, font_size);
    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        _characters.insert(std::pair<char, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void        Text::setupBuffers() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void 		Text::use() const {
	glUseProgram(_shader_program);
}

void        Text::_parseShaders(const char *vertex_shader_path, const char *frag_shader_path) {
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;
    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;

    v_shader_file.open(vertex_shader_path);
    f_shader_file.open(frag_shader_path);
    if (!v_shader_file.is_open() || !f_shader_file.is_open())
    {
        std::cout << "couln't open text shaders" << std::endl << vertex_shader_path << std::endl \
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

void 		Text::computeShaders() {
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	int success;
	char info_log[512];
    std::string vertex_shader_path = std::string(SRC_PATH) + "/Shaders/text_vertex_shader.glsl";
    std::string fragment_shader_path = std::string(SRC_PATH) + "/Shaders/text_fragment_shader.glsl";

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

void 		Text::setTexture(const char *tex_name, int tex_id) {
	glUniform1i(glGetUniformLocation(_shader_program, tex_name), tex_id);
}

void 		Text::setMat4(const char *loc_name, glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(_shader_program, loc_name), 1, GL_FALSE, &mat[0][0]);
}

void        Text::draw(std::string text, float x, float y, float scale, glm::vec3 color) {
    use();
    glUniform3f(glGetUniformLocation(_shader_program, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_vao);
    for (auto &c : text)
    {
        Character ch = _characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.tex_id);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Text::~Text() {
    for (auto &pair : _characters) {
        glDeleteTextures(1, &pair.second.tex_id);
    }
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteProgram(_shader_program);
}
