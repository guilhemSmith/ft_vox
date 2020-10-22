#include "Mesh.hpp"
#include "ft_vox.hpp"
#include <cstddef>

Mesh::Mesh() {
	_next_index = 0;
}

void 		Mesh::_createCube(Mesh::CubeData &data) {
	const glm::vec3 p1 = glm::vec3(data.pos.x , data.pos.y , data.pos.z + 1.0f);
	const glm::vec3 p2 = glm::vec3(data.pos.x + 1.0f, data.pos.y , data.pos.z + 1.0f);
	const glm::vec3 p3 = glm::vec3(data.pos.x + 1.0f, data.pos.y + 1.0f, data.pos.z + 1.0f);
	const glm::vec3 p4 = glm::vec3(data.pos.x , data.pos.y + 1.0f, data.pos.z + 1.0f);
	const glm::vec3 p5 = glm::vec3(data.pos.x + 1.0f, data.pos.y , data.pos.z );
	const glm::vec3 p6 = glm::vec3(data.pos.x , data.pos.y , data.pos.z );
	const glm::vec3 p7 = glm::vec3(data.pos.x , data.pos.y + 1.0f, data.pos.z );
	const glm::vec3 p8 = glm::vec3(data.pos.x + 1.0f, data.pos.y + 1.0f, data.pos.z );

	//front quad
	if (!data.f_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
        _vertices.push_back({p1, normal});
        _vertices.push_back({p2, normal});
        _vertices.push_back({p3, normal});
        _vertices.push_back({p4, normal});
        _next_index += 4;
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
    }

	//back quad
    if (!data.ba_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);
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
    }

	//left quad
    if (!data.l_neighbor) {
        glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
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
    } else {
        std::cout << "no l quad" << std::endl;
    }

	//right quad
	if (!data.r_neighbor) {
        glm::vec3 normal = glm::vec3(-1.0f, 0.0f, 0.0f);
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
    } else {
        std::cout << "no r quad" << std::endl;

	}

	//top quad
	if (!data.t_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
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
	}

	//bot quad
	if (!data.bo_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
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
    }
//	std::cout << _indices.size() << std::endl;
}

Mesh::Mesh(const std::array<std::array<std::array<char, 16>, 16>, 16> &cubes, glm::vec3 &pos) {
	for (int z = 0; z < 16; z++) {
		for (int y = 0; y < 16; y++) {
			for (int x = 0; x < 16; x++) {
                if (cubes[x][y][z] == 'e')
                    continue;
                Mesh::CubeData cube_data = Mesh::CubeData(glm::vec3(x + pos.x, y + pos.y, z + pos.z));
                if (x != 0 && cubes[x - 1][y][z] != 'e') {
                    std::cout << " rn found " << std::endl;
                    cube_data.r_neighbor = true;
                }
                if (x != 15 && cubes[x + 1][y][z] != 'e') {
                    std::cout << " ln found " << std::endl;
                    cube_data.l_neighbor = true;
                }
                if (y != 0 && cubes[x][y - 1][z] != 'e') {
                    std::cout << " bo found " << std::endl;
                    cube_data.bo_neighbor = true;
                }
                if (y != 15 && cubes[x][y + 1][z] != 'e') {
                    std::cout << " t found " << std::endl;
                    cube_data.t_neighbor = true;
                }
                if (z != 0 && cubes[x][y][z - 1] != 'e') {
                    std::cout << " ba found " << std::endl;
                    cube_data.ba_neighbor = true;
                }
                if (z != 15 && cubes[x][y][z + 1] != 'e') {
                    std::cout << " f found " << std::endl;
                    cube_data.f_neighbor = true;
                }
                if (cube_data.l_neighbor && cube_data.r_neighbor && cube_data.t_neighbor && cube_data.bo_neighbor && cube_data.f_neighbor && cube_data.ba_neighbor)
                    continue;
                //cube_data.pos = glm::vec3(glm::vec3(x + pos.x, y + pos.y, z + pos.z));
                _createCube(cube_data);

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
