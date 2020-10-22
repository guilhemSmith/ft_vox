#ifndef SHADER_HPP
# define SHADER_HPP

#include "ft_vox.hpp"

class Shader {
	private:
		unsigned int _shader_program;
	public:
		void 	computeShaders();
		void 	use(void) const;
		void 	setMat4(const char *, glm::mat4 &mat) const;
};

#endif
