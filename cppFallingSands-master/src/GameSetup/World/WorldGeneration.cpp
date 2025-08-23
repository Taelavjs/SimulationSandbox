#include "WorldGeneration.hpp"
#include "../../Elements/BaseElements/Pixel.hpp"
#include "../../Elements/Napalm.hpp"
#include "../../Elements/Oil.hpp"
#include "../../Elements/Rock.hpp"
#include "../../Elements/Sand.hpp"
#include "../../Elements/Smoke.hpp"
#include "../../Elements/Water.hpp"
#include "../../Utility/GlobalVariables.hpp"
#include "../../Algorithm/MarchingSquares.hpp"
#include <map>
#include <iomanip> 
#include <iostream>
#include <math.h>   
#include <random>

WorldGeneration::WorldGeneration(
	Sand* sand, Water* water, Rock* rock,
	Smoke* smoke, Napalm* napalm, Oil* oil)
	: emptyChunk() {

	this->sand = sand;
	this->water = water;
	this->rock = rock;
	this->smoke = smoke;
	this->napalm = napalm;
	this->oil = oil;

}


void WorldGeneration::generateBlock(SDL_Renderer* renderer) {
	for (int i = 0; i < GlobalVariables::worldChunkWidth; i++) {
		for (int j = 0; j < GlobalVariables::worldChunkWidth; j++) {
			Pixel* vec[GlobalVariables::chunkSize][GlobalVariables::chunkSize] = { nullptr };

			worldVecStore.emplace(std::piecewise_construct,
				std::forward_as_tuple(i, j),
				std::forward_as_tuple(Vector2D(i, j)));
		}
	}

	std::vector<float> terrainMap = ProceduralTerrainGen::createTerrain(GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth, GlobalVariables::worldChunkWidth * GlobalVariables::chunkSize);
	std::vector<float> noiseMap = ProceduralTerrainGen::createNoise(GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth, GlobalVariables::worldChunkWidth * GlobalVariables::chunkSize);

	for (auto& mapEntry : worldVecStore) {
		Chunk& currentChunk = mapEntry.second;
		pixelsToBlocks(noiseMap, mapEntry.first, currentChunk);
		generateCorridors(terrainMap, mapEntry.first, currentChunk);

		const Vector2D<int> currentCoords = mapEntry.first;
		currentChunk.createLinesTexture(MarchingSquares::run(currentChunk), renderer);
	}


}


void WorldGeneration::cleanUp() {
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
	int total = 0;
	std::setprecision(2);
	for (int row = 0; row < GlobalVariables::chunkSize; ++row)
	{
		for (int col = 0; col < GlobalVariables::chunkSize; ++col)
		{
			int globalRow = chunkStartY + row;
			int globalCol = chunkStartX + col;
			const float pixValue = noise[(globalRow) * (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) + (globalCol)];
			if (pixValue > -0.2f)
			{
				//vec[row][col] = rock->clone();
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
			if (chunkStartY < 15) {
				vec[row][col] = nullptr;
				continue;
			}

			if (vec[row][col] != nullptr) {
				continue;
			}
			double rng = getRandomDouble(0, 1);
			if (rng < 0.2 / 3) {
				vec[row][col] = water->clone();
			}
			else if (rng < 0.4 / 3)
			{
				vec[row][col] = sand->clone();
			}
			else if (rng < 0.6 / 3) {
				vec[row][col] = oil->clone();
			}
			else if (rng < 0.8 / 3) {
				vec[row][col] = napalm->clone();
			}
		}
	}
}

Chunk& WorldGeneration::getLocalVec() {
	return worldVecStore[Vector2D(0, 0)];
}

std::unordered_map<Vector2D<int>, Chunk>& WorldGeneration::getVecStore() {
	return worldVecStore;
}

Vector2D<float> WorldGeneration::getGlobalCoordinates(Vector2D<float> position) {
	return Vector2D<float>((position.x / GlobalVariables::chunkSize), (position.y / GlobalVariables::chunkSize));
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
void WorldGeneration::forceUpdatePixelFromGlobal(const Vector2D<int>& position) {
	Vector2D<int> chunkCoord(0, 0);
	Vector2D<int> localCoord(0, 0);

	chunkCoord.x = position.x / GlobalVariables::chunkSize;
	chunkCoord.y = position.y / GlobalVariables::chunkSize;

	localCoord.x = (position.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;
	localCoord.y = (position.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;

	auto chunkIt = worldVecStore.find(chunkCoord);
	if (chunkIt != worldVecStore.end()) {
		return chunkIt->second.getDirtyRect().expand(localCoord.x, localCoord.y);
	}
}


Pixel*& WorldGeneration::getPixelFromGlobal(const Vector2D<int>& position) {
	Vector2D<int> chunkCoord(0, 0);
	Vector2D<int> localCoord(0, 0);

	chunkCoord.x = position.x / GlobalVariables::chunkSize;
	chunkCoord.y = position.y / GlobalVariables::chunkSize;

	localCoord.x = (position.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;
	localCoord.y = (position.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;

	auto chunkIt = worldVecStore.find(chunkCoord);
	if (chunkIt != worldVecStore.end()) {
		return chunkIt->second[localCoord.y][localCoord.x];
	}
	static Pixel* nullpixel = nullptr;
	return nullpixel;
}

void WorldGeneration::clearPixelProcessed() {
	for (auto& mapEntry : worldVecStore) {
		Chunk& vec2D = mapEntry.second;
		Vector2D<int> globalCoords = mapEntry.first;

		for (int i = 0; i < GlobalVariables::chunkSize; i++) {
			for (int j = 0; j < GlobalVariables::chunkSize; j++) {
				Pixel*& pix = vec2D[i][j];
				if (pix != nullptr) {
					pix->setProcessed(false);
				}

			}
		}
	}
}

void WorldGeneration::swapTwoValues(Vector2D<int> pos1, Vector2D<int> pos2) {
	auto getCoords = [&](const Vector2D<int>& globalPos) {
		Vector2D<int> chunkCoord(
			std::floor(globalPos.x) / GlobalVariables::chunkSize,
			std::floor(globalPos.y) / GlobalVariables::chunkSize
		);
		Vector2D<int> localCoord(
			(globalPos.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize,
			(globalPos.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize
		);
		return std::make_pair(chunkCoord, localCoord);
		};

	auto [chunkCoord1, localCoord1] = getCoords(pos1);
	auto [chunkCoord2, localCoord2] = getCoords(pos2);

	auto it1 = worldVecStore.find(chunkCoord1);
	auto it2 = worldVecStore.find(chunkCoord2);
	if (it1 == worldVecStore.end() || it2 == worldVecStore.end()) return;

	Chunk& ch1 = it1->second;
	Chunk& ch2 = it2->second;

	ch1.getDirtyRect().expand(localCoord1.x, localCoord1.y);
	ch2.getDirtyRect().expand(localCoord2.x, localCoord2.y);

	if (ch1.size() == 0 || ch2.size() == 0) return;

	if (Pixel* px1 = ch1[localCoord1.y][localCoord1.x]) {
		px1->setProcessed(true);
	}
	if (Pixel* px2 = ch2[localCoord2.y][localCoord2.x]) {
		px2->setProcessed(true);
	}

	std::swap(ch1[localCoord1.y][localCoord1.x], ch2[localCoord2.y][localCoord2.x]);
	if (!(localCoord1.x == 0 || localCoord1.x == GlobalVariables::chunkSize - 1 ||
		localCoord1.y == 0 || localCoord1.y == GlobalVariables::chunkSize - 1)) return;
	std::vector<Vector2D<int>> relevantDirections;

	if (localCoord1.x == 0)
		relevantDirections.emplace_back(Vector2D<int>{ -1, 0 });
	else if (localCoord1.x == GlobalVariables::chunkSize - 1)
		relevantDirections.emplace_back(Vector2D<int>{ 1, 0 });

	if (localCoord1.y == 0)
		relevantDirections.emplace_back(Vector2D<int>{ 0, -1 });
	else if (localCoord1.y == GlobalVariables::chunkSize - 1)
		relevantDirections.emplace_back(Vector2D<int>{ 0, 1 });

	auto updateNeighbors = [&](const Vector2D<int>& globalPos, const std::vector<Vector2D<int>>& dirs) {
		for (const auto& dir : dirs) {
			auto [neighborChunkCoord, neighborLocalCoord] = getCoords(globalPos + dir);
			auto it = worldVecStore.find(neighborChunkCoord);
			if (it != worldVecStore.end()) {
				it->second.getDirtyRect().expand(neighborLocalCoord.x, neighborLocalCoord.y);
			}
		}
		};

	updateNeighbors(pos1, relevantDirections);

}

void WorldGeneration::burntSmoke(const int row, const int col) {
	Pixel*& pixelRef = getPixelFromGlobal(Vector2D(col, row));
	delete pixelRef;
	pixelRef = smoke->clone();
	pixelRef->setProcessed(true);
}