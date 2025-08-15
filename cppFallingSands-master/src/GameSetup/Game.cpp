#include "Game.hpp"
#include <string>
#include <future>
#include <iostream>
#include "../Playables/Player.hpp"
#include "../Textures/Sprite.hpp"
#include "Rendering.hpp"
#include "../Utility/GlobalVariables.hpp"
Game::Game()
	: isRunning(true), worldGeneration(), threadPool(2)
{
	sand = new Sand();
	water = new Water();
	rock = new Rock();
	smoke = new Smoke();
	oil = new Oil();
	napalm = new Napalm();
	SDL_GetKeyboardState(&numKeys);
	prevKeys = new Uint8[numKeys];
	SDL_PumpEvents();
	numChunksX = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeY;
	numChunksY = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeY;

}

Game::~Game()
{
	// SDL_DestroyRenderer(renderer);
	// SDL_DestroyWindow(window);
	// SDL_Quit();
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

void SquarePlace(Chunk& vec, int x, int y, Pixel* elm)
{
	int numToPlace{ 1 };
	for (int j = x - numToPlace; j < x + numToPlace; j++)
	{
		for (int k = y - numToPlace; k < y + numToPlace; k++)
		{
			if (j < 0 || k < 0 || k >= GlobalVariables::chunkSize || j >= GlobalVariables::chunkSize)
				continue;
			// if(rand() % 2 == 0) continue;

			delete vec[k][j];         // Delete existing object to avoid memory leak
			vec[k][j] = elm->clone(); // Assign new clone
		}
	}
}

void Game::handleEvents()
{
	const Uint8* e = SDL_GetKeyboardState(&numKeys);
	Chunk& localChunk = worldGeneration.getLocalVec();

	int x{}, y{};
	if (e)
	{
		SDL_GetMouseState(&x, &y);
		if (e[SDL_SCANCODE_P])
		{
			setRunning(false);
		}
		//else if (e[SDL_SCANCODE_A])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, water);
		//}
		//else if (e[SDL_SCANCODE_D])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, rock);
		//}
		//else if (e[SDL_SCANCODE_S])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, sand);
		//}
		//else if (e[SDL_SCANCODE_W])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, smoke);
		//}
		//else if (e[SDL_SCANCODE_Q])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, oil);
		//}
		//else if (e[SDL_SCANCODE_F])
		//{
		//	SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, napalm);
		//}

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

void Game::updateSequence(const int& row, const int& col)
{
	Pixel* pixel = worldGeneration.getPixelFromGlobal(Vector2D(col, row));
	if (pixel == nullptr)
		return;
	if (pixel->getProcessed())
		return;
	if (pixel->getIsFlammable())
	{
		if (worldGeneration.getPixelFromGlobal(Vector2D(col, row))->fireTick(worldGeneration, row, col))
		{
			worldGeneration.burntSmoke(row, col);
			return;
		};
	}
	pixel->update(row, col, GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth, GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth, worldGeneration);
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

	for (int i = 0; i < GlobalVariables::worldChunkWidth; i++) {
		for (int j = 0; j < GlobalVariables::worldChunkWidth; j++) {
			Chunk& chunk = worldGeneration.getVecStore()[{i, j}];
			auto& rect = chunk.getDirtyRect();

			if (!isFirstRun && !rect.getIsDirty()) {
				//chunk.drawLines(Rendering::getRenderer(), player->getPlayerRect());
				rect.reset();
				continue;
			}
			threadPool.enqueue([=, &chunk, &rect]() {
				//chunk.drawLines(Rendering::getRenderer(), player->getPlayerRect());
				Rendering::renderGrid(chunk, player, { i, j });
				rect.reset();
				});
		}
	}

}

void Game::render()
{
	RenderThreads();
	threadPool.wait_for_all_tasks();
	for (auto& ch : worldGeneration.getVecStore()) {
		ch.second.SDLRenderFunctions(Rendering::getRenderer());
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

