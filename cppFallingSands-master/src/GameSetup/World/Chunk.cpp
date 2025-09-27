#include "Chunk.hpp"
#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Utility/GlobalVariables.hpp"
#include <random>
#include <chrono>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/draw_constrained_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Regular_triangulation_2<K> Regular_triangulation;
// Instantion and deletion

Chunk::Chunk(Vector2D<int> chunkGlobalCoords)
	:globalCoords(chunkGlobalCoords), texture(nullptr), linesTexture(nullptr)
{
	pixels = new uint32_t[GlobalVariables::chunkSize * GlobalVariables::chunkSize];
	for (int i = 0; i < GlobalVariables::chunkSize; i++) {
		for (int j = 0; j < GlobalVariables::chunkSize; j++) {
			vec[i][j] = nullptr;
		}
	}
}

Chunk::Chunk()
	:globalCoords(Vector2D(999, 999))
{
	pixels = new uint32_t[GlobalVariables::chunkSize * GlobalVariables::chunkSize];
	texture = nullptr;
	linesTexture = nullptr;
	for (int i = 0; i < GlobalVariables::chunkSize; i++) {
		for (int j = 0; j < GlobalVariables::chunkSize; j++) {
			vec[i][j] = nullptr;
		}
	}
}
void Chunk::CreateBaseTextures(SDL_Renderer* renderer) {
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GlobalVariables::chunkSize, GlobalVariables::chunkSize);
	if (linesTexture == nullptr) {
		linesTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GlobalVariables::chunkSize, GlobalVariables::chunkSize);
	}

}

Chunk::~Chunk() {
	for (auto& row : vec) {
		for (Pixel* pix : row) {
			delete pix;
		}
	}

	if (texture) {
		SDL_DestroyTexture(texture);
	}
	delete[] pixels;
}
const Vector2D<int>& Chunk::getGlobalCoords() {
	return globalCoords;
}

// Render functions
void Chunk::render(SDL_Renderer* renderer, const SDL_Rect& playerRect) {


	if (texture == nullptr) {
		CreateBaseTextures(renderer);
	}
	resetPixels(0xFF000000);
	firstRender = false;
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			const uint32_t color = (vec[row][col] != nullptr) ? vec[row][col]->type->color : 0xFF000000;
			pixels[row * GlobalVariables::chunkSize + col] = color;

			if (vec[row][col] == nullptr)
				continue;
			vec[row][col]->setProcessed(false);
		}
	}
}

void Chunk::RenderDirtyRectToTexture(const SDL_Rect& playerRect) {
	const uint32_t greenColor = 0xFF00FF00;
	chunkBoundingBox& dirtyRect = getDirtyRect();
	const int& minX = dirtyRect.getMinX();
	const int& minY = dirtyRect.getMinY();
	const int& maxX = dirtyRect.getMaxX();
	const int& maxY = dirtyRect.getMaxY();

	if (dirtyRec.getIsDirty()) {
		for (int x = minX; x < maxX; ++x) {
			if (minY >= 0 && minY < GlobalVariables::chunkSize) {
				pixels[minY * GlobalVariables::chunkSize + x] = greenColor;
			}
			if (maxY >= 0 && maxY < GlobalVariables::chunkSize) {
				pixels[maxY * GlobalVariables::chunkSize + x] = greenColor;
			}
		}
		for (int y = minY; y < maxY; ++y) {
			if (minX >= 0 && minX < GlobalVariables::chunkSize) {
				pixels[y * GlobalVariables::chunkSize + minX] = greenColor;
			}
			if (maxX >= 0 && maxX < GlobalVariables::chunkSize) {
				pixels[y * GlobalVariables::chunkSize + maxX] = greenColor;
			}
		}
	}
	SDL_UpdateTexture(texture, NULL, pixels, GlobalVariables::chunkSize * sizeof(uint32_t));
}

void Chunk::UpdateChunkRenderLocation(const SDL_Rect& playerRect) {
	const int globalOffputX = globalCoords.x * GlobalVariables::chunkSize;
	const int globalOffputY = globalCoords.y * GlobalVariables::chunkSize;
	dstRect = {
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputX,
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputY,
		GlobalVariables::chunkSize,
		GlobalVariables::chunkSize
	};

	dstRect.x -= playerRect.x - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);
	dstRect.y -= playerRect.y - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);
}

void Chunk::resetPixels(const uint32_t& blackColor) {
	std::fill(pixels, pixels + (GlobalVariables::chunkSize * GlobalVariables::chunkSize), blackColor);
}


void Chunk::SDLRenderFunctions(SDL_Renderer* renderer, const SDL_Rect& playerRect) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(linesTexture, SDL_BLENDMODE_BLEND);
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	SDL_RenderCopy(renderer, linesTexture, NULL, &dstRect);

}

void Chunk::createLinesTexture(std::stack<std::pair<Vector2D<float>, Vector2D<float>>> _lines, SDL_Renderer* renderer) {
	lines = _lines;
	if (linesTexture == nullptr) {
		CreateBaseTextures(renderer);
	}

	SDL_SetRenderTarget(renderer, linesTexture);
	SDL_RenderClear(renderer);
	Uint8 r{}, g{}, b{}, a{};
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (!lines.empty()) {
		std::pair<Vector2D<float>, Vector2D<float>> line = lines.top();
		SDL_RenderDrawLineF(renderer,
			line.first.x,
			line.first.y,
			line.second.x,
			line.second.y);
		lines.pop();
	}
	SDL_RenderPresent(renderer);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

}
Uint8 getRandomColorComponent(std::mt19937& gen) {
	std::uniform_int_distribution<> distrib(0, 255);
	return (Uint8)distrib(gen);
}
using Polyline = std::vector<Vector2D<float>>;
void Chunk::drawSimplifiedPolygonsTexture(const std::vector<Polyline>& simplified_polygons, std::vector<std::tuple<K::Point_2, K::Point_2, K::Point_2>> tris, SDL_Renderer* renderer) {
	if (linesTexture == nullptr) {
		CreateBaseTextures(renderer);
		SDL_SetTextureBlendMode(linesTexture, SDL_BLENDMODE_BLEND);
	}

	unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);
	SDL_Texture* originalTarget = SDL_GetRenderTarget(renderer);
	Uint8 r{}, g{}, b{}, a{};
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	SDL_SetRenderTarget(renderer, linesTexture);
	for (const auto& polyline : simplified_polygons) {
		if (polyline.size() < 2) continue;
		Uint8 rand_r = getRandomColorComponent(gen);
		Uint8 rand_g = getRandomColorComponent(gen);
		Uint8 rand_b = getRandomColorComponent(gen);

		SDL_SetRenderDrawColor(renderer, rand_r, rand_g, rand_b, 255);
		for (size_t i = 0; i < polyline.size(); ++i) {
			const Vector2D<float>& p1 = polyline[i];

			size_t next_index = (i + 1) % polyline.size();
			const Vector2D<float>& p2 = polyline[next_index];

			SDL_RenderDrawLineF(renderer,
				p1.x, p1.y,
				p2.x, p2.y
			);

			if (next_index == 0) {
				break;
			}
		}
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

	for (const auto& tri : tris) {
		const K::Point_2& p1_local = std::get<0>(tri);
		const K::Point_2& p2_local = std::get<1>(tri);
		const K::Point_2& p3_local = std::get<2>(tri);

		float x1 = (p1_local.x());
		float y1 = (p1_local.y());
		float x2 = (p2_local.x());
		float y2 = (p2_local.y());
		float x3 = (p3_local.x());
		float y3 = (p3_local.y());

		SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
		SDL_RenderDrawLineF(renderer, x2, y2, x3, y3);
		SDL_RenderDrawLineF(renderer, x3, y3, x1, y1);
	}

	SDL_SetRenderTarget(renderer, originalTarget);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
int Chunk::size() {
	return GlobalVariables::chunkSize;
}
Pixel** Chunk::operator[](int x) { return vec[x]; }
chunkBoundingBox& Chunk::getDirtyRect() { return dirtyRec; };
SDL_Texture* Chunk::getTexture() { return texture; };
uint32_t* Chunk::getPixels() { return pixels; };

