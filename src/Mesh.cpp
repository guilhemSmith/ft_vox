#include "Mesh.hpp"
#include "ft_vox.hpp"
#include <cstddef>

Mesh::Mesh() {
	_vertices.push_back({glm::vec3(1.0f, 1.0f, -1.0f)});
	_vertices.push_back({glm::vec3(1.0f, -1.0f, -1.0f)});
	_vertices.push_back({glm::vec3(1.0f, 1.0f, 1.0f)});
	_vertices.push_back({glm::vec3(1.0f, -1.0f, 1.0f)});
	_vertices.push_back({glm::vec3(-1.0f, 1.0f, -1.0f)});
	_vertices.push_back({glm::vec3(-1.0f, -1.0f, -1.0f)});
	_vertices.push_back({glm::vec3(-1.0f, 1.0f, 1.0f)});
	_vertices.push_back({glm::vec3(-1.0f, -1.0f, 1.0f)});
	_indices.push_back(4);
	_indices.push_back(2);
	_indices.push_back(0);
	_indices.push_back(2);
	_indices.push_back(7);
	_indices.push_back(3);
	_indices.push_back(6);
	_indices.push_back(5);
	_indices.push_back(7);
	_indices.push_back(1);
	_indices.push_back(7);
	_indices.push_back(5);
	_indices.push_back(0);
	_indices.push_back(3);
	_indices.push_back(1);
	_indices.push_back(4);
	_indices.push_back(1);
	_indices.push_back(5);
	_indices.push_back(4);
	_indices.push_back(6);
	_indices.push_back(2);
	_indices.push_back(2);
	_indices.push_back(6);
	_indices.push_back(7);
	_indices.push_back(6);
	_indices.push_back(4);
	_indices.push_back(5);
	_indices.push_back(1);
	_indices.push_back(3);
	_indices.push_back(7);
	_indices.push_back(0);
	_indices.push_back(2);
	_indices.push_back(3);
	_indices.push_back(4);
	_indices.push_back(0);
	_indices.push_back(1);

	setupBuffers();
}

Mesh::Mesh(std::vector<char> voxels) {
	// TODO generate vertices from voxels
}

void 	Mesh::draw() {
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

void 	Mesh::setupBuffers() {
	glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Mesh::Vertex), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
 //   glEnableVertexAttribArray(1);	
 //   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glBindVertexArray(0);
}

std::ostream&	operator<<(std::ostream& os, Mesh& mesh) {

	//TODO print data for debug
	os << "Mesh: ";
	return os;
}
