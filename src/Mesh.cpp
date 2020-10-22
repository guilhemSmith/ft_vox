#include "Mesh.hpp"
#include "ft_vox.hpp"
#include <cstddef>
#include <algorithm>


Mesh::Mesh() {
	_next_index = 0;
}

void 		Mesh::_createCube(float x, float y, float z) {
	const glm::vec3 p1 = glm::vec3(x , y , z + 1.0f);
	const glm::vec3 p2 = glm::vec3(x + 1.0f, y , z + 1.0f);
	const glm::vec3 p3 = glm::vec3(x + 1.0f, y + 1.0f, z + 1.0f);
	const glm::vec3 p4 = glm::vec3(x , y + 1.0f, z + 1.0f);
	const glm::vec3 p5 = glm::vec3(x + 1.0f, y , z );
	const glm::vec3 p6 = glm::vec3(x , y , z );
	const glm::vec3 p7 = glm::vec3(x , y + 1.0f, z );
	const glm::vec3 p8 = glm::vec3(x + 1.0f, y + 1.0f, z );

	//front quad
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
	_vertices.push_back({p1, normal});
    _vertices.push_back({p2, normal});
    _vertices.push_back({p3, normal});
    _vertices.push_back({p4, normal});
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);

	//back quad
	normal = glm::vec3(0.0f, 0.0f, -1.0f);
    _vertices.push_back({p5, normal});
    _vertices.push_back({p6, normal});
    _vertices.push_back({p7, normal});
    _vertices.push_back({p8, normal});
	_next_index += 4;
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);

	//right quad
	normal = glm::vec3(1.0f, 0.0f, 0.0f);
    _vertices.push_back({p2, normal});
    _vertices.push_back({p5, normal});
    _vertices.push_back({p8, normal});
    _vertices.push_back({p3, normal});
	_next_index += 4;
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);
	//left quad
	normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    _vertices.push_back({p6, normal});
    _vertices.push_back({p1, normal});
    _vertices.push_back({p4, normal});
    _vertices.push_back({p7, normal});
	_next_index += 4;
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);
	//top quad
	normal = glm::vec3(0.0f, -1.0f, 0.0f);
    _vertices.push_back({p4, normal});
    _vertices.push_back({p3, normal});
    _vertices.push_back({p8, normal});
    _vertices.push_back({p7, normal});
	_next_index += 4;
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);
	//bot quad
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
    _vertices.push_back({p6, normal});
    _vertices.push_back({p5, normal});
    _vertices.push_back({p2, normal});
    _vertices.push_back({p1, normal});
	_next_index += 4;
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 1);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 0);
	_indices.push_back(_next_index + 2);
	_indices.push_back(_next_index + 3);
    _next_index += 4;
//	std::cout << _indices.size() << std::endl;
}

bool 	 isOnASide(int x, int y, int z)
{
	return x == 0 || x == 15 || y == 0 || y == 15 || z == 0 || z == 15;
}

Mesh::Mesh(const std::array<std::array<std::array<char, 16>, 16>, 16> &cubes, glm::vec3 &pos) {
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
				if (!isOnASide(x, y, z) && (cubes[x - 1][y][z] != 'e' && cubes[x + 1][y][z] != 'e' && cubes[x][y - 1][z] != 'e' && cubes[x][y + 1][z] != 'e' && cubes[x][y][z - 1] != 'e' && cubes[x][y][z + 1] != 'e'))
				{
//					std::cout << "cube is surrounded" << std::endl;
				}
				else if (cubes[x][y][z] != 'e')
				if (cubes[x][y][z] != 'e')
					_createCube(x + pos.x, y + pos.y ,z + pos.z);
			}
		}
	}
	_setupBuffers();
}

void 	Mesh::draw() {
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}

void 	Mesh::_setupBuffers() {
	glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Mesh::Vertex), &_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glBindVertexArray(0);
}

std::ostream&	operator<<(std::ostream& os, Mesh& mesh) {

	//TODO print data for debug
	os << "Mesh: ";
	return os;
}
