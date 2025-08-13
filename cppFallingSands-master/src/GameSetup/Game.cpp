#include "Game.hpp"
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Elements/Sand.hpp"
#include "../Elements/Water.hpp"
#include "../Elements/Rock.hpp"
#include "../Elements/Smoke.hpp"
#include "../Elements/Oil.hpp"
#include "../Elements/Napalm.hpp"
#include "Rendering.hpp"
#include "../Playables/Player.hpp"
#include "../Textures/Sprite.hpp"
#include "../Utility/GlobalVariables.hpp"

Game::Game()
	: isRunning(true), worldGeneration()
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
	worldGeneration.generateBlock();
	const char* playerSpritePath{ "C:\\Users\\taela\\source\\repos\\FallingSandsSumoSimulation\\cppFallingSands-master\\Sprites\\AnimationSheet_Character.png" };

	Rendering::setValues();
	int width{ 32 };
	int height{ 32 };
	int rows{ 9 };
	int cols{ 8 };
	Sprite* playerSprite = new Sprite(playerSpritePath, width, height, rows, cols);
	player = new Player(playerSprite);

}

void SquarePlace(Chunk& vec, int x, int y, Pixel* elm)
{
	int numToPlace{ 1 };
	for (int j = x - numToPlace; j < x + numToPlace; j++)
	{
		for (int k = y - numToPlace; k < y + numToPlace; k++)
		{
			if (j < 0 || k < 0 || k >= vec.size() || j >= vec[k].size())
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
	Chunk localChunk = worldGeneration.getLocalVec();

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
	pixel->update(row, col, GlobalVariables::chunkSize, GlobalVariables::chunkSize, worldGeneration);
}

void Game::worker(const Vector2D<int>& globalChunk, const int& startingChunkRow, const int& startingChunkCol, const Vector2D<float>& playerCoords) {
	std::vector<std::future<void>> futures;

	// iterating over the 8x8 sub-chunks
	for (int rowChunk = startingChunkRow; rowChunk < numChunksY; rowChunk += 2) {
		for (int colChunk = startingChunkCol; colChunk < numChunksX; colChunk += 2) {
			// Calculate the boundaries of the current 8x8 chunk
			int rowStart = globalChunk.y * GlobalVariables::chunkSize + (rowChunk * GlobalVariables::subChunkSizeY);
			int colStart = globalChunk.x * GlobalVariables::chunkSize + (colChunk * GlobalVariables::subChunkSizeY);
			int rowEnd = std::min(rowStart + GlobalVariables::subChunkSizeY, GlobalVariables::chunkSize * 2);
			int colEnd = std::min(colStart + GlobalVariables::subChunkSizeY, GlobalVariables::chunkSize * 2);

			// Perform the distance check for the entire 8x8 chunk
			float closestX = std::max((float)colStart, std::min(playerCoords.x, (float)colEnd));
			float closestY = std::max((float)rowStart, std::min(playerCoords.y, (float)rowEnd));
			float distance = std::sqrt(std::pow(playerCoords.x - closestX, 2) + std::pow(playerCoords.y - closestY, 2));

			if (distance > 200.0f) {
				continue;
			}

			for (int row = rowStart; row < rowEnd; ++row) {
				for (int col = colStart; col < colEnd; ++col) {
					updateSequence(row, col);
				}
			}
		}
	}
}
void Game::ChunkUpdateSkipping(Vector2D<int>& globalChunk, int startingChunkRow, int startingChunkCol, const Vector2D<float>& playerCoords) {

}

void Game::update()
{
	Chunk& vec = worldGeneration.getChunk(worldGeneration.getGlobalCoordinates(player->getCoordinates()));

	Vector2D dimensions = player->getDimensions();
	std::unordered_map<Vector2D<int>, Chunk>& chunks = worldGeneration.getVecStore();
	// Runs over each chunk, specifying what sub chunk the X and Y update sequence should start at
	// It allows for no two chunks directly next to each other to be updating at the same time, allowing for multicore processing
	// Without dirty reads/rights

	// Current issues with fire ticks and check for getIsFlammable - Unsure why
	const Vector2D<float>& playerCoords = player->getCoordinates();

	for (auto& mapEntry : chunks) {
		Chunk& vec2D = mapEntry.second;
		Vector2D globalCoords = mapEntry.first;

		worker(globalCoords, 1, 1, playerCoords);
		worker(globalCoords, 1, 0, playerCoords);
		worker(globalCoords, 0, 1, playerCoords);
		worker(globalCoords, 0, 0, playerCoords);
	}
	player->update(Rendering::getRenderer(), worldGeneration);
	worldGeneration.clearPixelProcessed();

}

void Game::render()
{
	std::unordered_map<Vector2D<int>, Chunk>& temp = worldGeneration.getVecStore();
	for (auto& mapEntry : temp) {
		Chunk& vec2D = mapEntry.second;
		Vector2D globalCoords = mapEntry.first;
		Rendering::renderGrid(vec2D, player, globalCoords);
	}
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
