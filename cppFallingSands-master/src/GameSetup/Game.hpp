#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include <SDL.h>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Elements/Sand.hpp"
#include "../Elements/Water.hpp"
#include "../Elements/Rock.hpp"
#include "../Elements/Smoke.hpp"
#include "../Elements/Oil.hpp"
#include "../Elements/Napalm.hpp"
#include "../../../SubChunkBoundingBox.hpp"
#include "World/WorldGeneration.hpp"

class Rendering;
class Player;
class Sprite;
class Chunk;

class Game {
public:
	Game();
	~Game();

	void init();
	void handleEvents();
	void update();
	void render();
	void clean();
	void setRunning(bool running) { isRunning = running; }
	bool getRunning() const { return isRunning; }
	void swapElements(int x, int y, int x2, int y2);
	double randomnumber();
	void updateSequence(const int& row, const int& col);
	void pixelsToBlocks(std::vector<float> pixels);
	void ChunkUpdateSkipping(Vector2D<int>& globalChunk, int startingChunkRow, int startingChunkCol, const Vector2D<float>& playerCoords);
	void worker(const Vector2D<int>& globalChunk, const Vector2D<float>& playerCoords, chunkBoundingBox& box);

	void generateCorridors(std::vector<float> pixels);
private:
	// int rendererScalex;
	// int rendererScaley;
	bool isRunning;
	WorldGeneration worldGeneration;

	int numChunksX;
	int numChunksY;
	Player* player;
	Uint8* prevKeys = nullptr;
	int numKeys = 0;
	Sand* sand;
	Water* water;
	Rock* rock;
	Smoke* smoke;
	Oil* oil;
	Napalm* napalm;
	bool isFirstRun{ true };
};

#endif // GAME_HPP
