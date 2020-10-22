#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include <glm/glm.hpp>
# include <ostream>
# include <array>

class Mesh {
private:
	struct Vertex {
		glm::vec3				position;
		glm::vec3				normal;
	};
	unsigned int 				_vao;
	unsigned int 				_vbo;
	unsigned int 				_ebo;

	std::vector<Mesh::Vertex>	_vertices;
	std::vector<unsigned int>	_indices;
	int 						_next_index;
	void 						_setupBuffers();
    void 	                    _createCube(float x, float y, float z);

public:
	//Mesh(std::vector<char> voxels);
	Mesh(const std::array<std::array<std::array<char, 16>, 16>, 16> &, glm::vec3 &);
	Mesh();//TODO remove

	void						draw();
};

std::ostream&	operator<<(std::ostream& os, Mesh& mesh);

#endif
