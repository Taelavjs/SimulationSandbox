

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../../../SubChunkBoundingBox.hpp"
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

	chunkBoundingBox& getDirtyRect() { return dirtyRec; };

private:
	std::vector<std::vector<Pixel*>> vec;
	Vector2D<int> globalCoords;
	chunkBoundingBox dirtyRec;
};

#endif /* CHUNK */
