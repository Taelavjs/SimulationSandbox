

#ifndef CHUNK
#define CHUNK
#include "../../Utility/Velocity.hpp"
#include <vector>
#include "../../../../SubChunkBoundingBox.hpp"
#include "../../../../cppFallingSands-master/src/Utility/GlobalVariables.hpp"
#include <SDL.h>
#include <stack>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/draw_constrained_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Regular_triangulation_2<K> Regular_triangulation;
class Pixel;
class WorldGeneration;
class Chunk {
	using Polyline = std::vector<Vector2D<float>>;

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
	void drawSimplifiedPolygonsTexture(const std::vector<Polyline>& simplified_polygons, std::vector<std::tuple<K::Point_2, K::Point_2, K::Point_2>> tris, SDL_Renderer* renderer);

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
