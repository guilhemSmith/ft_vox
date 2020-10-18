#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include <glm/glm.hpp>
# include <ostream>

class Mesh {
private:
	struct Vertex {
		glm::vec3				position;
		//glm::vec3				normal;
	};
	unsigned int 				_VAO;
	unsigned int 				_VBO;
	unsigned int 				_EBO;

	std::vector<Mesh::Vertex>	_vertices;
	std::vector<unsigned int>	_indices;
	void 						setupBuffers();

public:
	Mesh(std::vector<char> voxels);
	Mesh();//TODO remove

	void						draw();
};

std::ostream&	operator<<(std::ostream& os, Mesh& mesh);

#endif
