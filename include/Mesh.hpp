#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include <glm/glm.hpp>

class Mesh {
private:
	struct Vertex {
		glm::vec3				position;
		glm::vec3				normal;
	};

	std::vector<Mesh::Vertex>	_vertices;
	std::vector<unsigned int>	_indices;
public:
	Mesh(std::vector<char> voxels);

	void						render(glm::vec3 chunk_pos, glm::vec3 camera_pos, glm::vec3 camera_dir);
};

#endif