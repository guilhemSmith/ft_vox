#ifndef SHADER_HPP
# define SHADER_HPP

#include "ft_vox.hpp"

class Shader {
	private:
		unsigned int 	_shader_program;
		std::string     _vertex_shader_src;
    	std::string     _fragment_shader_src;
		void    		_parseShaders(const char*, const char*);
	public:
		void 	computeShaders();
		void 	use(void) const;
		void 	setMat4(const char *, glm::mat4 &mat) const;
		void 	setTexture(const char *, int);
};

#endif
