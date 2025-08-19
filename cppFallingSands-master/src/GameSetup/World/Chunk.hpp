

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
	void CreateBaseTextures(SDL_Renderer* renderer);

	int size();

	chunkBoundingBox& getDirtyRect();
	SDL_Texture* getTexture();
	uint32_t* getPixels();

	void resetPixels(const uint32_t& blackColor);
	void SDLRenderFunctions(SDL_Renderer* renderer, const SDL_Rect& playerRect);
	void createLinesTexture(std::stack<std::pair<Vector2D<float>, Vector2D<float>>> _lines, SDL_Renderer* renderer);
	void createLineTexture(SDL_Renderer* renderer);
	void RenderDirtyRectToTexture(const SDL_Rect& playerRect);
	void UpdateChunkRenderLocation(const SDL_Rect& playerRect);
private:
	Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize];
	Vector2D<int> globalCoords;
	chunkBoundingBox dirtyRec;
	SDL_Texture* texture;
	SDL_Texture* linesTexture;
	uint32_t* pixels;
	bool firstRender{ true };
	SDL_Rect dstRect;
	SDL_Rect lineDstRect;



	std::stack<std::pair<Vector2D<float>, Vector2D<float>>> lines;
};

#endif /* CHUNK */
