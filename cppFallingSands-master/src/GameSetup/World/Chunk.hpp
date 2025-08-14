

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../../../SubChunkBoundingBox.hpp"
#include "../../../../cppFallingSands-master/src/Utility/GlobalVariables.hpp"
#include <SDL.h>
class Pixel;
class WorldGeneration;
class Chunk {
public:
	Chunk();
	Chunk(Vector2D<int> chunkGlobalCoords);

	~Chunk();
	Chunk& operator=(Chunk&& other) = delete;

	void render(SDL_Renderer* renderer, const SDL_Rect& playerRect);
	const Vector2D<int>& getGlobalCoords();
	Pixel** operator[](int x);
	void CreateBaseTexture(SDL_Renderer* renderer);

	int size();

	chunkBoundingBox& getDirtyRect();
	SDL_Texture* getTexture();
	uint32_t* getPixels();

	void resetPixels(const uint32_t& blackColor);
private:
	Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize];
	Vector2D<int> globalCoords;
	chunkBoundingBox dirtyRec;
	SDL_Texture* texture;
	uint32_t* pixels;
	bool firstRender{ true };
};

#endif /* CHUNK */
