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
	: isRunning(true), worldGeneration(), threads()
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
	numChunksX = GlobalVariables::chunkSize / chunkSizeX;
	numChunksY = GlobalVariables::chunkSize / chunkSizeY;

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
	const char* playerSpritePath{ "Sprites/AnimationSheet_Character.png" };

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
		else if (e[SDL_SCANCODE_A])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, water);
		}
		else if (e[SDL_SCANCODE_D])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, rock);
		}
		else if (e[SDL_SCANCODE_S])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, sand);
		}
		else if (e[SDL_SCANCODE_W])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, smoke);
		}
		else if (e[SDL_SCANCODE_Q])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, oil);
		}
		else if (e[SDL_SCANCODE_F])
		{
			SquarePlace(localChunk, x / GlobalVariables::rendererScale, y / GlobalVariables::rendererScale, napalm);
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
	memcpy(prevKeys, e, numKeys);
	SDL_PumpEvents();
}

void Game::updateSequence(int& row, int& col)
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

void Game::worker(Vector2D<int> globalChunk, int startingChunkRow, int startingChunkCol, const Vector2D<float>& playerCoords)
{
	// Calculate the boundaries of the current chunk


	int chunkOffsetX = globalChunk.x * GlobalVariables::chunkSize;
	int chunkOffsetY = globalChunk.y * GlobalVariables::chunkSize;
	int rowStart = chunkOffsetX + (startingChunkRow * chunkSizeX);
	int colStart = chunkOffsetY + (startingChunkCol * chunkSizeY);
	int rowEnd = std::min(rowStart + chunkSizeY, GlobalVariables::chunkSize * 2);
	int colEnd = std::min(colStart + chunkSizeX, GlobalVariables::chunkSize * 2);
	if (std::abs(colEnd - playerCoords.x) > 150 || std::abs(rowEnd - playerCoords.y) > 150) return;
	if (std::abs(colStart - playerCoords.x) > 150 || std::abs(rowStart - playerCoords.y) > 150) return;

	// Update the 8x8 chunk
	for (int row = rowStart; row < rowEnd; ++row)
	{
		for (int col = colStart; col < colEnd; ++col)
		{
			if (std::abs(col - playerCoords.x) > 150 || std::abs(row - playerCoords.y) > 150) continue;
			updateSequence(row, col);
		}
	}
}
void Game::ChunkUpdateSkipping(Vector2D<int>& globalChunk, int startingChunkRow, int startingChunkCol, const Vector2D<float>& playerCoords)
{
	std::vector<std::future<void>> futures;

	int batchSize = std::max(1, numChunksY / 2);
	for (int rowChunk = startingChunkRow; rowChunk < numChunksY; rowChunk += 2)
	{
		for (int colChunk = startingChunkCol; colChunk < numChunksX; colChunk += 2)
		{
			futures.push_back(threads.enqueue([this, globalChunk, rowChunk, colChunk, playerCoords]()
				{
					this->worker(globalChunk, rowChunk, colChunk, playerCoords);
				}));

			if (futures.size() >= batchSize) {
				for (auto& future : futures) future.get();
				futures.clear();
			}
		}
	}

	// Wait for remaining tasks to finish
	for (auto& future : futures) future.get();
}


void Game::update()
{
	Chunk& vec = worldGeneration.getChunk(worldGeneration.getGlobalCoordinates(player->getCoordinates()));

	Vector2D dimensions = player->getDimensions();
	std::map<Vector2D<int>, Chunk>& chunks = worldGeneration.getVecStore();
	// Runs over each chunk, specifying what sub chunk the X and Y update sequence should start at
	// It allows for no two chunks directly next to each other to be updating at the same time, allowing for multicore processing
	// Without dirty reads/rights

	// Current issues with fire ticks and check for getIsFlammable - Unsure why
	const Vector2D playerCoords = player->getCoordinates();

	for (auto& mapEntry : chunks) {
		Chunk& vec2D = mapEntry.second;
		Vector2D globalCoords = mapEntry.first;
		ChunkUpdateSkipping(globalCoords, 1, 1, playerCoords);
		ChunkUpdateSkipping(globalCoords, 1, 0, playerCoords);
		ChunkUpdateSkipping(globalCoords, 0, 1, playerCoords);
		ChunkUpdateSkipping(globalCoords, 0, 0, playerCoords);
	}
	player->update(Rendering::getRenderer(), worldGeneration);
	worldGeneration.clearPixelProcessed();

}

void Game::render()
{
	std::map<Vector2D<int>, Chunk>& temp = worldGeneration.getVecStore();
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
