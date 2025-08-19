#include "Chunk.hpp"
#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Utility/GlobalVariables.hpp"
Chunk::Chunk(Vector2D<int> chunkGlobalCoords)
	:globalCoords(chunkGlobalCoords), texture(nullptr), lineTexture(nullptr)
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
	lineTexture = nullptr;
	for (int i = 0; i < GlobalVariables::chunkSize; i++) {
		for (int j = 0; j < GlobalVariables::chunkSize; j++) {
			vec[i][j] = nullptr;
		}
	}
}
void Chunk::CreateBaseTexture(SDL_Renderer* renderer) {
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, GlobalVariables::chunkSize, GlobalVariables::chunkSize);

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
	if (lineTexture) {
		SDL_DestroyTexture(lineTexture);
	}
	delete[] pixels;
}
const Vector2D<int>& Chunk::getGlobalCoords() {
	return globalCoords;
}

void Chunk::render(SDL_Renderer* renderer, const SDL_Rect& playerRect) {
	const int globalOffputX = globalCoords.x * GlobalVariables::chunkSize;
	const int globalOffputY = globalCoords.y * GlobalVariables::chunkSize;

	if (texture == nullptr) {
		CreateBaseTexture(renderer);
	}

	if (texture == nullptr)
	{
		std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
		return;
	}

	resetPixels(0xFF000000);
	//if (dirtyRec.getIsDirty()) {
	firstRender = false;
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			const uint32_t color = (vec[row][col] != nullptr) ? vec[row][col]->getColour() : 0xFF000000;
			pixels[row * GlobalVariables::chunkSize + col] = color;

			if (vec[row][col] == nullptr)
				continue;
			vec[row][col]->setProcessed(false);
		}
	}
	//}


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


	dstRect = {
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputX,
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputY,
		GlobalVariables::chunkSize,
		GlobalVariables::chunkSize
	};

	// Change 'playerRect.x' and 'playerRect.y' to 'playerRect->x' and 'playerRect->y'
	dstRect.x -= playerRect.x - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);
	dstRect.y -= playerRect.y - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);


	// Move opposite to player



}
chunkBoundingBox& Chunk::getDirtyRect() { return dirtyRec; };
SDL_Texture* Chunk::getTexture() { return texture; };
uint32_t* Chunk::getPixels() { return pixels; };

int Chunk::size() {
	//if (vec.empty() || vec[0].empty()) {
	//	return 0;
	//}
	return GlobalVariables::chunkSize;
}
void Chunk::resetPixels(const uint32_t& blackColor) {
	//std::fill(pixels, pixels + (GlobalVariables::chunkSize * GlobalVariables::chunkSize), blackColor);

}
Pixel** Chunk::operator[](int x) {
	return vec[x];
}

void Chunk::SDLRenderFunctions(SDL_Renderer* renderer) {
	SDL_UpdateTexture(texture, NULL, pixels, GlobalVariables::chunkSize * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
}

void Chunk::setLines(std::stack<std::pair<Vector2D<float>, Vector2D<float>>> _lines, SDL_Renderer* renderer) {
	lines = _lines;

	if (lineTexture == nullptr) {
		createLineTexture(renderer);
	}
	if (renderer == nullptr) {
		// Handle the error gracefully, e.g., by logging a message
		return;
	}
	SDL_SetRenderTarget(renderer, lineTexture);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // transparent black
	SDL_RenderClear(renderer);
	// Set the drawing color for your lines
	Uint8 r{}, g{}, b{}, a{};
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// Draw the lines onto the texture
	while (!lines.empty()) {
		std::pair<Vector2D<float>, Vector2D<float>> line = lines.top();
		SDL_RenderDrawLineF(renderer,
			line.first.x,
			line.first.y,
			line.second.x,
			line.second.y);
		lines.pop();
	}

	// Reset the render target back to the default (the screen)
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

}

void Chunk::drawLines(SDL_Renderer* renderer, const SDL_Rect& playersRect) {
	if (lineTexture == nullptr) {
		return;
	}

	const int globalOffputX = globalCoords.x * GlobalVariables::chunkSize;
	const int globalOffputY = globalCoords.y * GlobalVariables::chunkSize;

	SDL_Rect lineDstRect = {
		globalOffputX,
		globalOffputY,
		GlobalVariables::chunkSize,
		GlobalVariables::chunkSize
	};
	lineDstRect.x -= playersRect.x - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);
	lineDstRect.y -= playersRect.y - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);

	SDL_RenderCopy(renderer, lineTexture, NULL, &lineDstRect);
}


void Chunk::createLineTexture(SDL_Renderer* renderer) {
	if (lineTexture != nullptr) {
		SDL_DestroyTexture(lineTexture);
	}
	lineTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		GlobalVariables::chunkSize,
		GlobalVariables::chunkSize);
}