

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../../../SubChunkBoundingBox.hpp"
#include "../../../../cppFallingSands-master/src/Utility/GlobalVariables.hpp"
#include <SDL.h>
#include <stack>
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
	void SDLRenderFunctions(SDL_Renderer* renderer);
	void setLines(std::stack<std::pair<Vector2D<float>, Vector2D<float>>> _lines, SDL_Renderer* renderer);
	void drawLines(SDL_Renderer* renderer, const SDL_Rect& playersRect);
	void createLineTexture(SDL_Renderer* renderer);
private:
	Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize];
	Vector2D<int> globalCoords;
	chunkBoundingBox dirtyRec;
	SDL_Texture* texture;
	SDL_Texture* lineTexture;

	uint32_t* pixels;
	bool firstRender{ true };
	SDL_Rect dstRect;
	SDL_Rect lineDstRect;

	std::stack<std::pair<Vector2D<float>, Vector2D<float>>> lines;
};

#endif /* CHUNK */
