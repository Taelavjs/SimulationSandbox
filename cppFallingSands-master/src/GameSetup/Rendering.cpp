#include "Rendering.hpp"
#include "../Utility/GlobalVariables.hpp"
SDL_Window* Rendering::window = nullptr;
SDL_Renderer* Rendering::renderer = nullptr;
int Rendering::offsetX = 0;
int Rendering::offsetY = 0;
uint32_t Rendering::blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 30, 255);
uint32_t Rendering::redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);

void Rendering::setValues() {

	SDL_Init(SDL_INIT_VIDEO);
	std::cout << "Everything SDL Initialized Correctly" << '\n';

	Rendering::window = SDL_CreateWindow(GlobalVariables::title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
	Rendering::renderer = SDL_CreateRenderer(Rendering::window, -1, 0);
	SDL_SetRenderTarget(renderer, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderSetScale(Rendering::renderer, GlobalVariables::rendererScale, GlobalVariables::rendererScale);
	SDL_RenderDrawPoint(Rendering::renderer, 5, 5);
	SDL_RenderPresent(Rendering::renderer);
	SDL_PumpEvents();
}

Rendering::~Rendering()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Rendering::castRay(uint32_t* pixels, SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int startX, int startY, int endX, int endY) {
	int dx = abs(endX - startX);
	int dy = abs(endY - startY);
	int sx = startX < endX ? 1 : -1;
	int sy = startY < endY ? 1 : -1;
	int err = dx - dy;
	int e2;
	int count{ 0 };
	int limit{ 3 };
	while (true) {

		if (startX >= 0 && startX < GlobalVariables::chunkSize && startY >= 0 && startY < GlobalVariables::chunkSize) {
			if ((vec[startX][startY] == nullptr || vec[startX][startY]->getIsMoveable()) && limit > 0) {
				limit--;
				pixels[startX * GlobalVariables::chunkSize + startY] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 182, 130, 124, 90);
			}
			else if (vec[startX][startY] != nullptr || limit <= 0) {
				count++;
				if (count == 2) break;
			};
		}

		if (startX == endX && startY == endY) break;
		e2 = 2 * err;
		if (e2 > -dy) { err -= dy; startX += sx; }
		if (e2 < dx) { err += dx; startY += sy; }
	}
}


void Rendering::castRays(uint32_t* pixels, SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int row, int col) {
	int rays = 360; // Number of rays to cast
	for (int i = 0; i < rays; ++i) {
		float angle = i * (2 * M_PI / rays);
		int endX = row + GlobalVariables::chunkSize * cos(angle);
		int endY = col + GlobalVariables::chunkSize * sin(angle);
		castRay(pixels, renderer, vec, row, col, endX, endY);
	}
}

void Rendering::renderGrid(Chunk& vec, Player* player, Vector2D<int> globalCoords) {
	const int globalOffputX = globalCoords.x * GlobalVariables::chunkSize;
	const int globalOffputY = globalCoords.y * GlobalVariables::chunkSize;

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, GlobalVariables::chunkSize, GlobalVariables::chunkSize);
	if (texture == nullptr)
	{
		std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
		return;
	}
	uint32_t* pixels = new uint32_t[GlobalVariables::chunkSize * GlobalVariables::chunkSize];

	int z = 0;
	std::fill(pixels, pixels + (GlobalVariables::chunkSize * GlobalVariables::chunkSize), blackColor);
	const Vector2D<float>& playerCoords = player->getCoordinates();
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		if (std::abs(row + globalOffputY - playerCoords.y) > 300) continue;

		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			if (std::abs(col + globalOffputX - playerCoords.x) > 300) continue;
			uint32_t color = (vec[row][col] != nullptr) ? vec[row][col]->getColour() : blackColor;

			pixels[row * GlobalVariables::chunkSize + col] = color;
			Rendering::ShowSubchunks(pixels, row, col);

			if (vec[row][col] == nullptr)
				continue;
			vec[row][col]->setProcessed(false);
			z++;
		}

	}
	// The green color we will use for the border (Opaque Green).
	const uint32_t greenColor = 0xFF00FF00;

	// --- Start of new logic to draw the dirty rect border ---
	chunkBoundingBox& dirtyRect = vec.getDirtyRect();
	if (dirtyRect.getIsDirty()) {
		int minX = dirtyRect.getMinX();
		int minY = dirtyRect.getMinY();
		int maxX = dirtyRect.getMaxX();
		int maxY = dirtyRect.getMaxY();

		// Loop to draw the top and bottom borders
		for (int x = minX; x < maxX; ++x) {
			// Top border
			if (minY >= 0 && minY < GlobalVariables::chunkSize) {
				pixels[minY * GlobalVariables::chunkSize + x] = greenColor;
			}
			// Bottom border
			if (maxY >= 0 && maxY < GlobalVariables::chunkSize) {
				pixels[maxY * GlobalVariables::chunkSize + x] = greenColor;
			}
		}

		// Loop to draw the left and right borders
		for (int y = minY; y < maxY; ++y) {
			// Left border
			if (minX >= 0 && minX < GlobalVariables::chunkSize) {
				pixels[y * GlobalVariables::chunkSize + minX] = greenColor;
			}
			// Right border
			if (maxX >= 0 && maxX < GlobalVariables::chunkSize) {
				pixels[y * GlobalVariables::chunkSize + maxX] = greenColor;
			}
		}
	}

	dirtyRect.reset();
	const SDL_Rect& AABB = player->getPlayerRect();
	Rendering::offsetX = AABB.x - 5;
	Rendering::offsetY = AABB.y - 1;
	SDL_Rect dstRect = { ((GlobalVariables::chunkSize / 2) - Rendering::offsetX) + globalOffputX,((GlobalVariables::chunkSize / 2) - Rendering::offsetY) + globalOffputY, GlobalVariables::chunkSize, GlobalVariables::chunkSize };

	SDL_UpdateTexture(texture, NULL, pixels, GlobalVariables::chunkSize * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	delete[] pixels;
	SDL_DestroyTexture(texture);
}

void Rendering::ShowSubchunks(uint32_t* pixels, const int& row, const int& col) {
	if (row % GlobalVariables::subChunkSizeX == 0 || col % GlobalVariables::subChunkSizeX == 0) pixels[row * GlobalVariables::chunkSize + col] = redColor;
}

void Rendering::renderPlayer(Player* player) {
	SDL_Rect AABB = player->getPlayerRect();
	player->renderPlayer(renderer, GlobalVariables::chunkSize);
	std::stack<SDL_Rect> toRender = player->getStackRender();
	while (!toRender.empty()) {
		SDL_Rect cube = toRender.top();
		SDL_RenderDrawRect(renderer, &cube);
		toRender.pop();
	}
}

void Rendering::showRendering() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

