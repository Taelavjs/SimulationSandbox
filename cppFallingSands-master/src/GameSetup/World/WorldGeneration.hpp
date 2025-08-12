#ifndef WORLDGENERATION
#define WORLDGENERATION
#include "../../Utility/ProceduralTerrainGen.hpp"
#include "../../Utility/Velocity.hpp"
#include <iostream>
#include <iomanip> // Required for std::setprecision
#include <vector>
#include <map>
#include "Chunk.hpp"
#include <math.h>       /* fmod */
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
    void generateBlock();
    void pixelsToBlocks(std::vector<float> noise, Vector2D worldQuad, Chunk &chunk);
    void generateCorridors(std::vector<float> noise, Vector2D worldQuad, Chunk &chunk);
    Chunk& getLocalVec();
    Vector2D getGlobalCoordinates(Vector2D position);
    std::map<Vector2D, Chunk>& getVecStore();
    Vector2D getGlobalCoordinates(int chunkX, int chunkY, int localX, int localY, int chunkSizeX, int chunkSizeY);
    Chunk& getChunk(Vector2D chunkGlobalCoord);
    Pixel*& getPixelFromGlobal(Vector2D position);
    void clearPixelProcessed();
    void swapTwoValues(Vector2D pos1, Vector2D pos2);
    void burntSmoke(const int row, const int col);
private:
    std::map<Vector2D, Chunk> worldVecStore;
    Sand *sand;
    Water *water;
    Rock *rock;
    Smoke *smoke;
    Oil *oil;
    Napalm *napalm;
    int width{0};
    Chunk emptyChunk;
};

#endif /* WORLDGENERATION */
