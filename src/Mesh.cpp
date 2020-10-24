#include "Mesh.hpp"
#include "Chunk.hpp"
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
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
    }

	//back quad
    if (!data.ba_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);
        _vertices.push_back({p5, normal});
        _vertices.push_back({p6, normal});
        _vertices.push_back({p7, normal});
        _vertices.push_back({p8, normal});
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
    }

	//left quad
    if (!data.l_neighbor) {
        glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
        _vertices.push_back({p2, normal});
        _vertices.push_back({p5, normal});
        _vertices.push_back({p8, normal});
        _vertices.push_back({p3, normal});
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
    }

	//right quad
	if (!data.r_neighbor) {
        glm::vec3 normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        _vertices.push_back({p6, normal});
        _vertices.push_back({p1, normal});
        _vertices.push_back({p4, normal});
        _vertices.push_back({p7, normal});
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
    }

	//top quad
	if (!data.t_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
        _vertices.push_back({p4, normal});
        _vertices.push_back({p3, normal});
        _vertices.push_back({p8, normal});
        _vertices.push_back({p7, normal});
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
	}

	//bot quad
	if (!data.bo_neighbor) {
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        _vertices.push_back({p6, normal});
        _vertices.push_back({p5, normal});
        _vertices.push_back({p2, normal});
        _vertices.push_back({p1, normal});
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 1);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 0);
        _indices.push_back(_next_index + 2);
        _indices.push_back(_next_index + 3);
        _next_index += 4;
    }
//	std::cout << _indices.size() << std::endl;
}

Mesh::Mesh(const std::array<std::array<std::array<char, 32>, 32>, 32> &cubes, glm::vec3 &pos) {
	for (int z = 0; z < Chunk::SIZE; z++) {
		for (int y = 0; y < Chunk::SIZE; y++) {
			for (int x = 0; x < Chunk::SIZE; x++) {
                if (cubes[x][y][z] == Chunk::Voxel::Empty)
                    continue;
                Mesh::CubeData cube_data = Mesh::CubeData(glm::vec3(x + pos.x, y + pos.y, z + pos.z));
                if (x != 0 && cubes[x - 1][y][z] != Chunk::Voxel::Empty) {
                    cube_data.r_neighbor = true;
                }
                if (x != Chunk::SIZE - 1 && cubes[x + 1][y][z] != Chunk::Voxel::Empty) {
                    cube_data.l_neighbor = true;
                }
                if (y != 0 && cubes[x][y - 1][z] != Chunk::Voxel::Empty) {
                    cube_data.bo_neighbor = true;
                }
                if (y != Chunk::SIZE - 1 && cubes[x][y + 1][z] != Chunk::Voxel::Empty) {
                    cube_data.t_neighbor = true;
                }
                if (z != 0 && cubes[x][y][z - 1] != Chunk::Voxel::Empty) {
                    cube_data.ba_neighbor = true;
                }
                if (z != Chunk::SIZE - 1 && cubes[x][y][z + 1] != Chunk::Voxel::Empty) {
                    cube_data.f_neighbor = true;
                }
                if (cube_data.l_neighbor && cube_data.r_neighbor && cube_data.t_neighbor && cube_data.bo_neighbor && cube_data.f_neighbor && cube_data.ba_neighbor)
                    continue;
                _createCube(cube_data);

			}
		}
	}
	_setupBuffers();
}

void 	Mesh::draw() {
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)nullptr);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, normal));

    glBindVertexArray(0);
}

std::ostream&	operator<<(std::ostream& os, Mesh& mesh) {

	//TODO print data for debug
	os << "Mesh: ";
	return os;
}
