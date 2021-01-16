#ifndef MESH_HPP
# define MESH_HPP

# include <vector>
# include <queue>
# include <glm/glm.hpp>
# include <ostream>
# include <array>
# include <memory>

class Chunk;

class Mesh {
private:
	struct Vertex {
		glm::vec3				position;
		glm::vec3				normal;
		glm::vec2               tex_coords;
		int                     texture;
	};
	struct CubeData {
	    glm::vec3               pos;
	    int                     texture;
	    bool                    r_neighbor;
        bool                    l_neighbor;
        bool                    f_neighbor;
        bool                    ba_neighbor;
        bool                    t_neighbor;
        bool                    bo_neighbor;
        explicit CubeData(glm::vec3 pos, int texture) : pos(pos), texture(texture) {
            r_neighbor = false;
            l_neighbor = false;
            f_neighbor = false;
            ba_neighbor = false;
            bo_neighbor = false;
            t_neighbor = false;
        };
	};
	unsigned int 				_vao{};
	unsigned int 				_vbo{};
	unsigned int 				_ebo{};
	bool						_is_ready;

	std::vector<Mesh::Vertex>	_vertices;
	std::vector<unsigned int>	_indices;
	int 						_next_index{};
	void 						_setupBuffers();
    void 	                    _createCube(Mesh::CubeData &);

	static std::queue<std::array<unsigned int, 3>>	_BUFFER_POOL;

public:
	Mesh();
	~Mesh();

	void						init(const std::array<std::array<std::array<char, 32>, 32>, 32> &cubes, glm::vec3 &pos, std::array<std::shared_ptr<Chunk>, 6> &);
	void						draw();
	void 						clearBuffers();
	bool						isReady() const;

	static void					clearBufferPool();
};

#endif
