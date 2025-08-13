#include "Game.hpp"
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

void Game::worker(
	const Vector2D<int>& globalChunk,
	const std::vector<SubChunkBoundingBox>& subchunkGroup,
	const Vector2D<float>& playerCoords
) {
	// Iterate over the precalculated subchunk bounding boxes
	for (const auto& box : subchunkGroup) {
		// Calculate the boundaries of the current subchunk
		int rowStart = globalChunk.y * GlobalVariables::chunkSize + box.subChunkRowStart;
		int colStart = globalChunk.x * GlobalVariables::chunkSize + box.subChunkColStart;
		int rowEnd = globalChunk.y * GlobalVariables::chunkSize + box.subChunkRowEnd;
		int colEnd = globalChunk.x * GlobalVariables::chunkSize + box.subChunkColEnd;

		// Perform the distance check for the entire subchunk
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
void Game::ChunkUpdateSkipping(Vector2D<int>& globalChunk, int startingChunkRow, int startingChunkCol, const Vector2D<float>& playerCoords) {

}
void Game::update() {
	// Pre-calculate the groups once, storing the result
	static const auto groups = precalculate_groups();

	// ... (Your other setup code) ...
	std::unordered_map<Vector2D<int>, Chunk>& chunks = worldGeneration.getVecStore();
	const Vector2D<float>& playerCoords = player->getCoordinates();

	for (auto& mapEntry : chunks) {
		Vector2D globalCoords = mapEntry.first;

		// Call the worker function four times, once for each precalculated group
		worker(globalCoords, groups[0], playerCoords);
		worker(globalCoords, groups[1], playerCoords);
		worker(globalCoords, groups[2], playerCoords);
		worker(globalCoords, groups[3], playerCoords);
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

std::vector<std::vector<SubChunkBoundingBox>> Game::precalculate_groups() {
	std::vector<std::vector<SubChunkBoundingBox>> groups(4);

	int num_subchunks_x = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeX;
	int num_subchunks_y = GlobalVariables::chunkSize / GlobalVariables::subChunkSizeY;

	for (int y = 0; y < num_subchunks_y; ++y) {
		for (int x = 0; x < num_subchunks_x; ++x) {
			int group_index = (x % 2) * 2 + (y % 2);

			// Calculate the bounding box for the current subchunk
			int rowStart = y * GlobalVariables::subChunkSizeY;
			int rowEnd = rowStart + GlobalVariables::subChunkSizeY;
			int colStart = x * GlobalVariables::subChunkSizeX;
			int colEnd = colStart + GlobalVariables::subChunkSizeX;

			// Create a SubChunkBoundingBox object and add it to the correct group
			groups[group_index].emplace_back(rowStart, rowEnd, colStart, colEnd);
		}
	}
	return groups;
}