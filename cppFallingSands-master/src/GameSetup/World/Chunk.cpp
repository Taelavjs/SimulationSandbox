#include "Chunk.hpp"
#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Utility/GlobalVariables.hpp"
Chunk::Chunk(Vector2D<int> chunkGlobalCoords)
	:globalCoords(chunkGlobalCoords), texture(nullptr)
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


	SDL_Rect dstRect = {
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputX,
		((GlobalVariables::chunkSize / GlobalVariables::worldChunkWidth)) + globalOffputY,
		GlobalVariables::chunkSize,
		GlobalVariables::chunkSize
	};

	// Change 'playerRect.x' and 'playerRect.y' to 'playerRect->x' and 'playerRect->y'
	dstRect.x -= playerRect.x - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);
	dstRect.y -= playerRect.y - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2);


	// Move opposite to player

	SDL_UpdateTexture(texture, NULL, pixels, GlobalVariables::chunkSize * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);

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