#ifndef SKYBOX_HPP
# define SKYBOX_HPP

#include <ft_vox.hpp>

class Skybox {
	private:
		unsigned int 					_shader_program;
		std::string     				_vertex_shader_src;
    	std::string     				_fragment_shader_src;
		std::vector<glm::vec3> 			_vertices;
		std::vector<unsigned int> 		_indices;
		unsigned int 					_vao;
		unsigned int 					_vbo;
		unsigned int 					_ebo;
		void    						_parseShaders(const char*, const char*);

	public:
		Skybox();
		~Skybox();
		void 							setupBuffers();
		void 							computeShaders();
		void 							use(void) const;
		void 							setMat4(const char *, glm::mat4 &mat) const;
		void 							setTexture(const char *, int);
		void 							draw();
};

#endif
