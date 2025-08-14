

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../../../SubChunkBoundingBox.hpp"
#include <SDL.h>
class Pixel;
class WorldGeneration;
class Chunk {
public:
	Chunk();
	Chunk(Vector2D<int> chunkGlobalCoords, std::vector<std::vector<Pixel*>> chunkVec);

	~Chunk();
	Chunk& operator=(Chunk&& other) noexcept;

	void render(SDL_Renderer* renderer, const SDL_Rect& playerRect);
	const Vector2D<int>& getGlobalCoords();
	std::vector<Pixel*>& operator[](int x);
	void CreateBaseTexture(SDL_Renderer* renderer);

	int size();

	chunkBoundingBox& getDirtyRect();
	SDL_Texture* getTexture();
	uint32_t* getPixels();

	void resetPixels(const uint32_t& blackColor);
private:
	std::vector<std::vector<Pixel*>> vec;
	Vector2D<int> globalCoords;
	chunkBoundingBox dirtyRec;
	SDL_Texture* texture;
	uint32_t* pixels;
	bool firstRender{ true };
};

#endif /* CHUNK */
