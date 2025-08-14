#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL.h"

#include "SDL_image.h"
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Playables/Player.hpp"
#include "../Utility/Velocity.hpp"
class Player;
class Rendering {
public:
	static void setValues();
	static void renderGrid(Chunk& vec, Player* player, Vector2D<int> globalCoords);
	static SDL_Renderer* getRenderer();
	static void renderPlayer(Player* player);
	static void showRendering();


private:
	static void castRay(uint32_t* pixels, SDL_Renderer* renderer, const Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize], int startX, int startY, int endX, int endY);
	static void castRays(uint32_t* pixels, SDL_Renderer* renderer, const Pixel*& vec, int row, int col);
	static void ShowSubchunks(uint32_t* pixels, const int& row, const int& col);

private:

	Rendering();
	~Rendering();
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static int offsetX;
	static int offsetY;
	static uint32_t blackColor;
	static uint32_t redColor;
	static SDL_Rect camera;
};

#endif /* RENDERING_HPP */
