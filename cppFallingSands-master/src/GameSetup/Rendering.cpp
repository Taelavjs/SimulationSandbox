#include "Rendering.hpp"
#include "../Utility/GlobalVariables.hpp"
#include <stack>
#include <iostream>
#include <vector>

SDL_Window* Rendering::window = nullptr;
SDL_Renderer* Rendering::renderer = nullptr;
int Rendering::offsetX = 0;
int Rendering::offsetY = 0;
uint32_t Rendering::blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 30, 255);
uint32_t Rendering::redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);
SDL_Rect Rendering::camera = { 0,0, GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth * GlobalVariables::rendererScale, GlobalVariables::rendererScale * GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth };
// Revised Rendering.cpp

void Rendering::setValues() {
	SDL_Init(SDL_INIT_VIDEO);
	std::cout << "Everything SDL Initialized Correctly" << '\n';

	int windowWidth = GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth;
	int windowHeight = GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth;

	camera.w = windowWidth * GlobalVariables::rendererScale;
	camera.h = windowHeight * GlobalVariables::rendererScale;

	Rendering::window = SDL_CreateWindow(GlobalVariables::title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, camera.w, camera.h, SDL_WINDOW_SHOWN);
	Rendering::renderer = SDL_CreateRenderer(Rendering::window, -1, 0);

	SDL_RenderSetScale(Rendering::renderer, GlobalVariables::rendererScale, GlobalVariables::rendererScale);

	SDL_SetRenderTarget(renderer, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	SDL_RenderPresent(Rendering::renderer);
	SDL_PumpEvents();
}

Rendering::~Rendering()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Rendering::castRay(uint32_t* pixels, SDL_Renderer* renderer, const Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize], int startX, int startY, int endX, int endY) {
	//int dx = abs(endX - startX);
	//int dy = abs(endY - startY);
	//int sx = startX < endX ? 1 : -1;
	//int sy = startY < endY ? 1 : -1;
	//int err = dx - dy;
	//int e2;
	//int count{ 0 };
	//int limit{ 3 };
	//while (true) {

	//	if (startX >= 0 && startX < GlobalVariables::chunkSize && startY >= 0 && startY < GlobalVariables::chunkSize) {
	//		if ((vec[startX][startY] == nullptr || vec[startX][startY]->getIsMoveable()) && limit > 0) {
	//			limit--;
	//			pixels[startX * GlobalVariables::chunkSize + startY] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 182, 130, 124, 90);
	//		}
	//		else if (vec[startX][startY] != nullptr || limit <= 0) {
	//			count++;
	//			if (count == 2) break;
	//		};
	//	}

	//	if (startX == endX && startY == endY) break;
	//	e2 = 2 * err;
	//	if (e2 > -dy) { err -= dy; startX += sx; }
	//	if (e2 < dx) { err += dx; startY += sy; }
	//}
}


void Rendering::castRays(uint32_t* pixels, SDL_Renderer* renderer, const Pixel*& vec, int row, int col) {
	//int rays = 360; // Number of rays to cast
	//for (int i = 0; i < rays; ++i) {
	//	float angle = i * (2 * M_PI / rays);
	//	int endX = row + GlobalVariables::chunkSize * cos(angle);
	//	int endY = col + GlobalVariables::chunkSize * sin(angle);
	//	castRay(pixels, renderer, vec, row, col, endX, endY);
	//}
}

void Rendering::renderGrid(Chunk& vec, Player* player, Vector2D<int> globalCoords) {
	//const SDL_Rect& playerPos = player->getPlayerRect();
	//vec.render(renderer, playerPos);

}
SDL_Renderer* Rendering::getRenderer() {
	return renderer;
};
// 1. Creating texture of chunk

// 2. Displaying it opposite of players position

// 3. Deleting Texture

void Rendering::ShowSubchunks(uint32_t* pixels, const int& row, const int& col) {
	if (row % GlobalVariables::subChunkSizeX == 0 || col % GlobalVariables::subChunkSizeX == 0) pixels[row * GlobalVariables::chunkSize + col] = redColor;
}

void Rendering::renderPlayer(Player* player) {
	SDL_Rect AABB = player->getPlayerRect();
	player->renderPlayer(renderer, GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth);
	std::stack<SDL_Rect> toRender = player->getStackRender();
	//while (!toRender.empty()) {
	//	SDL_Rect cube = toRender.top();
	//	SDL_RenderDrawRect(renderer, &cube);
	//	toRender.pop();
	//}
}

void Rendering::showRendering() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

