#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"
#include "../../Utility/GlobalVariables.hpp"
WorldGeneration::WorldGeneration()
	: emptyChunk()
{
	sand = new Sand();
	water = new Water();
	rock = new Rock();
	smoke = new Smoke();
	napalm = new Napalm();
	oil = new Oil();
}

WorldGeneration::~WorldGeneration() {
	delete sand;
	delete water;
	delete rock;
	delete oil;
	delete smoke;
	delete napalm;
	// Clear the map itself
	worldVecStore.clear();
}

void WorldGeneration::generateBlock() {
	std::vector<std::vector<Pixel*>> vec(GlobalVariables::chunkSize, std::vector<Pixel*>(GlobalVariables::chunkSize));
	worldVecStore[Vector2D(0, 0)] = Chunk(Vector2D(0, 0), vec);
	worldVecStore[Vector2D(0, 1)] = Chunk(Vector2D(0, 1), vec);
	worldVecStore[Vector2D(1, 0)] = Chunk(Vector2D(1, 0), vec);
	worldVecStore[Vector2D(1, 1)] = Chunk(Vector2D(1, 1), vec);


	std::vector<float> noiseMap = ProceduralTerrainGen::createNoise(GlobalVariables::chunkSize, GlobalVariables::chunkSize);
	std::vector<float> terrainMap = ProceduralTerrainGen::createTerrain(GlobalVariables::chunkSize, GlobalVariables::chunkSize);

	for (auto& mapEntry : worldVecStore) {
		Chunk& chunk = mapEntry.second;
		pixelsToBlocks(noiseMap, mapEntry.first, chunk);
		generateCorridors(terrainMap, mapEntry.first, chunk);
	}
}


void WorldGeneration::cleanUp() {
	delete sand;
	delete water;
	delete rock;
	delete oil;
	delete smoke;
	delete napalm;
}

void printVector(const std::vector<float>& vec) {
	std::cout << "Vector elements:\n";
	std::cout << std::fixed << std::setprecision(2);
	for (const auto& element : vec) {
		std::cout << element << " ";
	}
	std::cout << std::endl;
}

void WorldGeneration::pixelsToBlocks(std::vector<float> noise, Vector2D<int> worldQuad, Chunk& vec)
{
	int chunkStartX = worldQuad.x * GlobalVariables::chunkSize;
	int chunkStartY = worldQuad.y * GlobalVariables::chunkSize;
	std::cout << worldQuad.x << worldQuad.y << '\n';
	int total = 0;
	std::setprecision(2);
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			int globalRow = chunkStartY + row;
			int globalCol = chunkStartX + col;
			const float pixValue = noise[(globalRow) * (GlobalVariables::chunkSize * 2) + (globalCol)];
			if (pixValue > -0.2f)
			{
				vec[row][col] = rock->clone();
				total += 1;
			}
		}
	}
}

double getRandomDouble(double min, double max) {
	// Create a random device and a random number generator
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the distribution range
	std::uniform_real_distribution<> dis(min, max);

	// Generate and return a random number
	return dis(gen);
}

void WorldGeneration::generateCorridors(std::vector<float> noise, Vector2D<int> worldQuad, Chunk& vec)
{
	int chunkStartX = worldQuad.x * GlobalVariables::chunkSize;
	int chunkStartY = worldQuad.y * GlobalVariables::chunkSize;
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			int globalRow = chunkStartY + row;
			int globalCol = chunkStartX + col;
			const float pixValue = noise[(globalRow) * (GlobalVariables::chunkSize * 2) + (globalCol)];
			if (vec[row][col] != nullptr && vec[row][col]->getIsSolid() && pixValue > 0.6)
			{
				vec[row][col] = nullptr;
			}
			if (row < 30)
			{
				vec[row][col] = nullptr;
			}
		}
	}

	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			if (vec[row][col] != nullptr) {
				continue;
			}
			double rng = getRandomDouble(0, 1);
			if (rng < 0.2 / 6) {
				vec[row][col] = water->clone();
			}
			else if (rng < 0.4 / 6)
			{
				vec[row][col] = sand->clone();
			}
			else if (rng < 0.6 / 6) {
				vec[row][col] = oil->clone();
			}
			else if (rng < 0.8 / 6) {
				vec[row][col] = napalm->clone();
			}
		}
	}
}

Chunk& WorldGeneration::getLocalVec() {
	return worldVecStore[Vector2D(0, 0)];
}

std::map<Vector2D<int>, Chunk>& WorldGeneration::getVecStore() {
	return worldVecStore;
}

Vector2D<float> WorldGeneration::getGlobalCoordinates(Vector2D<float> position) {
	Vector2D<float> result(0, 0);
	result.x = std::floor(position.x / GlobalVariables::chunkSize);
	result.y = std::floor(position.y / GlobalVariables::chunkSize);

	std::cout << "Global Coordinates : " << result.x << " " << result.y << '\n';
	return result;
}

Vector2D<int> WorldGeneration::getGlobalCoordinates(int chunkX, int chunkY, int localX, int localY, int chunkSizeX, int chunkSizeY) {
	return Vector2D(chunkX * GlobalVariables::chunkSize + localX, chunkY * GlobalVariables::chunkSize + localY);
}

Chunk& WorldGeneration::getChunk(Vector2D<float> chunkGlobalCoord) {
	Vector2D<int> floatToIntVec = Vector2D<int>((int)chunkGlobalCoord.x, (int)chunkGlobalCoord.y);
	if (worldVecStore.find(floatToIntVec) != worldVecStore.end()) {
		return worldVecStore[floatToIntVec];
	}
	return emptyChunk;
}

// Make the class able to give the correct chunk using global coordinates
// Rather than swapping between global and local, much easier

Pixel*& WorldGeneration::getPixelFromGlobal(Vector2D<int> position) {
	// GlobalVariables::chunkSize = GlobalVariables::screeenSize;
	Vector2D<int> chunkCoord(0, 0);
	Vector2D<int> localCoord(0, 0);

	chunkCoord.x = position.x / GlobalVariables::chunkSize;
	chunkCoord.y = position.y / GlobalVariables::chunkSize;

	localCoord.x = (position.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;
	localCoord.y = (position.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;

	if (worldVecStore.find(chunkCoord) != worldVecStore.end()) {
		return worldVecStore[chunkCoord][localCoord.y][localCoord.x];
	}

	static Pixel* nullpixel = nullptr;

	return nullpixel;
}

void WorldGeneration::clearPixelProcessed() {
	for (auto& mapEntry : worldVecStore) {
		Chunk& vec2D = mapEntry.second;
		Vector2D<int> globalCoords = mapEntry.first;

		for (int i = 0; i < vec2D.size(); i++) {
			for (int j = 0; j < vec2D[i].size(); j++) {
				Pixel*& pix = vec2D[i][j];
				if (pix != nullptr) {
					pix->setProcessed(false);
				}

			}
		}
	}
}

void WorldGeneration::swapTwoValues(Vector2D<int> pos1, Vector2D<int> pos2) {
	// Helper function to get chunk and local coordinates
	auto getCoords = [&](const Vector2D<int>& globalPos) {
		Vector2D<int> chunkCoord(
			static_cast<int>(std::floor(static_cast<float>(globalPos.x) / GlobalVariables::chunkSize)),
			static_cast<int>(std::floor(static_cast<float>(globalPos.y) / GlobalVariables::chunkSize))
		);
		Vector2D<int> localCoord(
			(globalPos.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize,
			(globalPos.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize
		);
		return std::make_pair(chunkCoord, localCoord);
		};

	auto [chunkCoord1, localCoord1] = getCoords(pos1);
	auto [chunkCoord2, localCoord2] = getCoords(pos2);

	// Check if both chunks exist before proceeding
	if (worldVecStore.find(chunkCoord1) == worldVecStore.end() ||
		worldVecStore.find(chunkCoord2) == worldVecStore.end()) {
		return;
	}

	Chunk& ch1 = worldVecStore[chunkCoord1];
	Chunk& ch2 = worldVecStore[chunkCoord2];

	// Check if chunks are empty, although the find() check above should handle this
	if (ch1.size() == 0 || ch2.size() == 0) {
		return;
	}

	// Set "processed" flag for non-null pixels
	if (ch1[localCoord1.y][localCoord1.x] != nullptr) {
		ch1[localCoord1.y][localCoord1.x]->setProcessed(true);
	}
	if (ch2[localCoord2.y][localCoord2.x] != nullptr) {
		ch2[localCoord2.y][localCoord2.x]->setProcessed(true);
	}

	// Perform the swap
	Pixel* temp = ch1[localCoord1.y][localCoord1.x];
	ch1[localCoord1.y][localCoord1.x] = ch2[localCoord2.y][localCoord2.x];
	ch2[localCoord2.y][localCoord2.x] = temp;
}

void WorldGeneration::burntSmoke(const int row, const int col) {
	Pixel*& pixelPtr = getPixelFromGlobal(Vector2D(col, row));
	delete pixelPtr;
	pixelPtr = smoke->clone();
	pixelPtr->setProcessed(true);
}
