

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
class Pixel;

class Chunk {
public:
	Chunk();
	Chunk(Vector2D<int> chunkGlobalCoords, std::vector<std::vector<Pixel*>> chunkVec);
	~Chunk();
	Vector2D<int> getGlobalCoords();
	std::vector<Pixel*>& operator[](int x) {
		return vec[x];
	}

	int size() {
		if (vec.empty() || vec[0].empty()) {
			return 0;
		}
		return (int)vec[0].size();
	}

private:
	std::vector<std::vector<Pixel*>> vec;
	Vector2D<int> globalCoords;

};

#endif /* CHUNK */
