#ifndef WORLDGENERATION
#define WORLDGENERATION
#include "../../Utility/ProceduralTerrainGen.hpp"
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "Chunk.hpp"
#include <unordered_map>
class Chunk;
class Napalm;
class Oil;
class Smoke;
class Rock;
class Water;
class Sand;
class Pixel;
class Chunk;
class WorldGeneration {
public:
	WorldGeneration();
	~WorldGeneration();
	void cleanUp();
	void generateBlock(SDL_Renderer* renderer);
	void pixelsToBlocks(std::vector<float> noise, Vector2D<int> worldQuad, Chunk& chunk);
	void generateCorridors(std::vector<float> noise, Vector2D<int> worldQuad, Chunk& chunk);
	Chunk& getLocalVec();
	Vector2D<float> getGlobalCoordinates(Vector2D<float> position);
	std::unordered_map<Vector2D<int>, Chunk>& getVecStore();
	Vector2D<int> getGlobalCoordinates(int chunkX, int chunkY, int localX, int localY, int chunkSizeX, int chunkSizeY);
	Chunk& getChunk(Vector2D<float> chunkGlobalCoord);
	void forceUpdatePixelFromGlobal(const Vector2D<int>& position);
	Pixel*& getPixelFromGlobal(const Vector2D<int>& position);
	void clearPixelProcessed();
	void swapTwoValues(Vector2D<int> pos1, Vector2D<int> pos2);
	void burntSmoke(const int row, const int col);
private:
	std::unordered_map<Vector2D<int>, Chunk> worldVecStore;
	Sand* sand;
	Water* water;
	Rock* rock;
	Smoke* smoke;
	Oil* oil;
	Napalm* napalm;
	int width{ 0 };
	Chunk emptyChunk;
};

#endif /* WORLDGENERATION */
