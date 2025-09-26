#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "SDL.h"
#include "SDL_image.h"
#include "./GameSetup/Game.hpp"
#include "./Elements/BaseElements/Pixel.hpp"
#include "Textures/Sprite.hpp"
#include "./utility/ProceduralTerrainGen.hpp"
#include "./utility/GlobalVariables.hpp"
#include <box2d/box2d.h>

class Sprite;
class Pixel;
class Game;

const int blackColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 0, 255);
const int whiteColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255);
const int redColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 0, 0, 255);
const int greenColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 255, 0, 255);
const int blueColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 0, 255, 255);
const int yellowColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 201, 217, 98, 255);
const int vertColor = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 50, 168, 127, 255);

struct Timer
{
	Uint64 previous_ticks{};
	float elapsed_seconds{};

	void tick()
	{
		const Uint64 current_ticks{ SDL_GetPerformanceCounter() };
		const Uint64 delta{ current_ticks - previous_ticks };
		previous_ticks = current_ticks;
		static const Uint64 TICKS_PER_SECOND{ SDL_GetPerformanceFrequency() };
		elapsed_seconds = delta / static_cast<float>(TICKS_PER_SECOND);
	}
};

int main(int argc, char* argv[])
{
	std::vector<float> pixels(GlobalVariables::chunkSize * GlobalVariables::chunkSize);
	Game game;

	game.init();
	b2WorldDef worldDef = b2DefaultWorldDef();
	worldDef.gravity = { 0.0f, -10.0f };
	b2WorldId worldId = b2CreateWorld(&worldDef);
	b2BodyDef groundBodyDef = b2DefaultBodyDef();
	groundBodyDef.position = { 0.0f, -10.0f };
	b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
	b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
	b2ShapeDef groundShapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

	const int fps = 60;
	const double timeBetweenFrames = 1.0 / fps;
	static Timer system_timer;
	double accumulated_seconds{ 0.0 };
	int i{ 0 };

	while (game.getRunning())
	{
		system_timer.tick();
		accumulated_seconds += system_timer.elapsed_seconds;


		if (std::isgreater(accumulated_seconds, timeBetweenFrames)) {
			GlobalVariables::setDeltaTime(accumulated_seconds);
			std::cout << accumulated_seconds << '\n';
			accumulated_seconds = -timeBetweenFrames;

			i++;
			game.handleEvents();
			game.update();
			game.render();
		}
	}



	SDL_Quit();
	return 0;
}
