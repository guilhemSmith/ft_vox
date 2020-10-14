#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>

class Chunk {
	private:
		std::vector<char> 	_cubes;
		Mesh 				_mesh;
		

		
	public:
		void 	render();
		void 	remesh();
};


#endif
