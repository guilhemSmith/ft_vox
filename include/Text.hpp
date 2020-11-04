#ifndef TEXT_HPP
# define TEXT_HPP

#include <ft_vox.hpp>
#include <Time.hpp>
#include <map>

class Text {
	private:
        struct Character {
            unsigned int tex_id;  // ID handle of the glyph texture
            glm::ivec2   size;       // Size of glyph
            glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
            long         advance;    // Offset to advance to next glyph
        };
        std::map<char, Character>       _characters;
		unsigned int 					_shader_program;
		std::string     				_vertex_shader_src;
    	std::string     				_fragment_shader_src;
		unsigned int 					_vao;
		unsigned int 					_vbo;
		void    						_parseShaders(const char*, const char*);

	public:
		Text(unsigned int);
		~Text();
		void 							setupBuffers();
		void 							computeShaders();
		void 							use(void) const;
		void 							setMat4(const char *, glm::mat4 &mat) const;
		void 							setTexture(const char *, int);
		void 							draw(std::string text, float x, float y, float scale, glm::vec3 color);
};

#endif
