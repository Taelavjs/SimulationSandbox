#ifndef RENDERING_HPP
#define RENDERING_HPP
#include "SDL.h"
#include "SDL_image.h"

#include <iostream>
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Playables/Player.hpp"
#include <stack>
#include "../Utility/Velocity.hpp"
class Player;
class GlobalVariables;
class Rendering {
public:

	static void renderGrid(Chunk& vec, Player* player, Vector2D<int> globalCoords);
	static void castRay(uint32_t* pixels, SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int startX, int startY, int endX, int endY);
	static void castRays(uint32_t* pixels, SDL_Renderer* renderer, const std::vector<std::vector<Pixel*>>& vec, int row, int col);
	static SDL_Renderer* getRenderer() {
		return renderer;
	};
	static void setValues();
	static void renderPlayer(Player* player);
	static void showRendering();
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
};

#endif /* RENDERING_HPP */
