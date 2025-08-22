#include "Game.hpp"
#include <string>
#include <future>
#include <iostream>
#include "../Playables/Player.hpp"
#include "../Textures/Sprite.hpp"
#include "Rendering.hpp"
#include "../Utility/GlobalVariables.hpp"
#include <random>

// INSTANTIATION AND DELETION

Game::Game()
	: isRunning(true),
	threadPool(2),

	// Initialize PixelType members directly.
	rockType(false, false, true, false, false, 20, 10, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 150, 170, 100, 255)),
	sandType(true, false, true, false, false, 2, 5, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 200, 200, 0, 255)),
	waterType(true, true, false, false, false, 8, 5, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 50, 180, 200)),
	oilType(true, true, false, false, true, 14, 10, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 155, 155, 21, 255)),
	napalmType(true, true, false, false, true, 8, 3, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 50, 25, 200)),
	smokeType(true, false, false, true, false, 5, 2, 0, SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 130, 130, 130, 255)),

	rock(new Rock(&rockType)),
	sand(new Sand(&sandType)),
	water(new Water(&waterType)),
	oil(new Oil(&oilType)),
	napalm(new Napalm(&napalmType)),
	smoke(new Smoke(&smokeType)),

	worldGeneration(sand, water, rock, smoke, napalm, oil) {

	// Initializing keyboard state
	SDL_GetKeyboardState(&numKeys);
	prevKeys = new Uint8[numKeys];
	SDL_PumpEvents();

	// Initializing chunk variables
	numChunksX = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeY;
	numChunksY = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeY;
}

Game::~Game()
{
	delete sand;
	delete water;
	delete rock;
	delete oil;
	delete smoke;
	delete napalm;
}

void Game::init()
{
	const char* playerSpritePath{ "C:\\Users\\taela\\source\\repos\\FallingSandsSumoSimulation\\cppFallingSands-master\\Sprites\\AnimationSheet_Character.png" };

	Rendering::setValues();

	int width{ 32 };
	int height{ 32 };
	int rows{ 9 };
	int cols{ 8 };
	Sprite* playerSprite = new Sprite(playerSpritePath, width, height, rows, cols);
	player = new Player(playerSprite);
	worldGeneration.generateBlock(Rendering::getRenderer());


}

// Player Clicking



void Game::handleEvents()
{
	const Uint8* e = SDL_GetKeyboardState(&numKeys);

	int xScreen{}, yScreen{};
	if (e)
	{
		SDL_GetMouseState(&xScreen, &yScreen);
		int x = static_cast<int>((xScreen / GlobalVariables::rendererScale) - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2) + player->getPlayerRect().x);
		int y = static_cast<int>((yScreen / GlobalVariables::rendererScale) - ((GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2) + player->getPlayerRect().y);
		if (e[SDL_SCANCODE_P])
		{
			setRunning(false);
		}
		else if (e[SDL_SCANCODE_A])
		{
			playerPlacedPixelsSquare(x, y, water);
		}
		else if (e[SDL_SCANCODE_D])
		{
			playerPlacedPixelsSquare(x, y, oil);
		}
		else if (e[SDL_SCANCODE_S])
		{
			playerPlacedPixelsSquare(x, y, napalm);
		}
		else if (e[SDL_SCANCODE_W])
		{
			playerPlacedPixelsSquare(x, y, sand);
		}
		else if (e[SDL_SCANCODE_Q])
		{
			playerPlacedPixelsSquare(x, y, smoke);
		}
		else if (e[SDL_SCANCODE_F])
		{
			playerPlacedPixelsSquare(x, y, rock);
		}

		for (int i = 0; i < numKeys; ++i) {
			if (e[i] == 1) {
				player->playerInputHandler((SDL_Scancode)i);
			}
		}
	}
	for (int i = 0; i < numKeys; ++i) {
		if (prevKeys[i] == 1 && e[i] == 0) {
			player->playerReleaseHandler((SDL_Scancode)i);
		}
	}
	if (numKeys > 0) {
		memcpy(prevKeys, e, numKeys);
	}
	SDL_PumpEvents();
}

void Game::playerPlacedPixelsSquare(const int x, const int y, Pixel* type) {
	for (int i = x - 10; i < x + 10; i++) {
		for (int j = y - 10; j < y + 10; j++) {
			if (i < 0 || i >= GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth ||
				j < 0 || j >= GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) continue;
			Pixel*& pix = worldGeneration.getPixelFromGlobal({ i, j });
			if (pix != nullptr) {
				delete pix;
			}
			pix = type->clone();
			worldGeneration.forceUpdatePixelFromGlobal({ i, j });
		}
	}
}

// UPDATING  GRID

void Game::updateSequence(const int& row, const int& col)
{
	Pixel* pixel = worldGeneration.getPixelFromGlobal(Vector2D(col, row));
	if (pixel == nullptr)
		return;
	if (pixel->getProcessed())
		return;
	if (pixel->getIsFlammable())
	{
		if (worldGeneration.getPixelFromGlobal(Vector2D(col, row))->getOnFire()) {
			worldGeneration.forceUpdatePixelFromGlobal({ col, row });
		}

		if (worldGeneration.getPixelFromGlobal(Vector2D(col, row))->fireTick(worldGeneration, row, col))
		{
			worldGeneration.burntSmoke(row, col);
			return;
		};
	}
	pixel->update(row, col, worldGeneration);
}

void Game::worker(const Vector2D<int>& globalChunk, const Vector2D<float>& playerCoords, chunkBoundingBox& box) {
	int minRow = box.getMinY();
	int maxRow = box.getMaxY();
	int minCol = box.getMinX();
	int maxCol = box.getMaxX();
	for (int row = globalChunk.y * GlobalVariables::chunkSize + minRow; row <= globalChunk.y * GlobalVariables::chunkSize + maxRow; ++row) {
		const Sint16 globalRow = globalChunk.y * GlobalVariables::chunkSize + row;

		for (int col = globalChunk.x * GlobalVariables::chunkSize + minCol; col <= globalChunk.x * GlobalVariables::chunkSize + maxCol; ++col) {
			const Sint16 globalCol = globalChunk.x * GlobalVariables::chunkSize + col;

			//float distance = std::sqrt(std::pow(playerCoords.x - globalCol, 2) + std::pow(playerCoords.y - globalRow, 2));

			//if (distance > 200.0f) {
			//	continue;
			//}
			updateSequence(row, col);
		}
	}


}

void Game::MultiThreadWorker(const int startIndex, const int startYIndex,
	const Vector2D<float>& playerCoords) {
	for (int i = startIndex; i < GlobalVariables::worldChunkWidth; i += 2) {
		for (int j = startYIndex; j < GlobalVariables::worldChunkWidth; j += 2) {
			chunkBoundingBox& box = worldGeneration.getVecStore()[{i, j}].getDirtyRect();
			if (isFirstRun || box.getIsDirty()) {
				threadPool.enqueue([this, &box, playerCoords, i, j]() {
					this->worker({ i, j }, playerCoords, box);
					});
			}

		}
	}
	threadPool.wait_for_all_tasks();
}

void Game::update() {
	Chunk& vec = worldGeneration.getChunk(worldGeneration.getGlobalCoordinates(player->getCoordinates()));

	Vector2D dimensions = player->getDimensions();
	const Vector2D<float>& playerCoords = player->getCoordinates();
	MultiThreadWorker(0, 0, playerCoords);
	MultiThreadWorker(1, 0, playerCoords);
	MultiThreadWorker(0, 1, playerCoords);
	MultiThreadWorker(1, 1, playerCoords);

	//for (auto& mapEntry : chunks) {
	//	auto& rec = mapEntry.second.getDirtyRect();
	//	if (!isFirstRun && !rec.getIsDirty()) return;
	//	worker(mapEntry.first, playerCoords, rec);
	//}
	player->update(Rendering::getRenderer(), worldGeneration);
	worldGeneration.clearPixelProcessed();
}

void Game::RenderThreads() {
}

void Game::render()
{
	for (auto& ch : worldGeneration.getVecStore()) {
		auto& rect = ch.second.getDirtyRect();
		ch.second.render(Rendering::getRenderer(), player->getPlayerRect());
		ch.second.UpdateChunkRenderLocation(player->getPlayerRect());
		ch.second.RenderDirtyRectToTexture(player->getPlayerRect());
		ch.second.SDLRenderFunctions(Rendering::getRenderer(), player->getPlayerRect());
		rect.reset();
	}

	isFirstRun == false;
	Rendering::renderPlayer(player);
	Rendering::showRendering();
}

void Game::clean() {
};

double Game::randomnumber()
{
	static std::default_random_engine rng;
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rng);
}

