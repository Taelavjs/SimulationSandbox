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
#include "../../Algorithm/Douglas.hpp"
#include <map>
#include <iomanip> 
#include <iostream>
#include <math.h>   
#include <random>
#include <box2d/box2d.h>
#include "../../../../WorldManager.hpp"

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
		std::vector<std::vector<Vector2D<float>>> simplifiedPolyLines;
		Chunk& currentChunk = mapEntry.second;
		pixelsToBlocks(noiseMap, mapEntry.first, currentChunk);
		generateCorridors(terrainMap, mapEntry.first, currentChunk);

		const Vector2D<int> currentCoords = mapEntry.first;
		std::stack<std::pair<Vector2D<float>, Vector2D<float>>> marchingSquaresResults = MarchingSquares::run(currentChunk);
		std::vector<std::pair<Vector2D<float>, Vector2D<float>>> segments;
		while (!marchingSquaresResults.empty()) {
			segments.push_back(marchingSquaresResults.top());
			marchingSquaresResults.pop();
		}

		std::unordered_map<Vector2D<float>, std::vector<Vector2D<float>>> adjacency;
		for (auto& [p1, p2] : segments) {
			adjacency[p1].push_back(p2);
			adjacency[p2].push_back(p1);
		}

		auto connectedLineGroups = Douglas::SegmentingLines(adjacency);
		std::vector<std::vector<Vector2D<float>>> allSimplifiedPolylines;
		Vector2dVector a;
		Vector2dVector result;
		for (const auto& group : connectedLineGroups) {
			if (group.size() < 3) continue; // Skip if too few vertices for a polygon
			std::vector<Vector2D<float>> simplified = Douglas::DouglasPeucker(group, 1.0f);
			if (group.front() == group.back() && simplified.front() != simplified.back()) {
				simplified.push_back(simplified.front());
			}
			if (simplified.size() < 3) continue;

			for (const auto& val : simplified) {
				a.push_back(Vector2d(val.x, val.y));
			}
			Triangulate::Process(a, result);
		}
		currentChunk.drawSimplifiedPolygonsTexture(simplifiedPolyLines, result, renderer);
	}

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { 65.0f, 4.0f };
	b2BodyId bodyId = b2CreateBody(WorldManager::GetInstance().GetWorldId(), &bodyDef);
	b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);
	for (int i = 0; i < 16; ++i) {
		WorldManager::GetInstance().step();
		b2Vec2 position = b2Body_GetPosition(bodyId);
		b2Rot rotation = b2Body_GetRotation(bodyId);
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation));

		// Draw the box without rotation:
		SDL_Rect rect;
		rect.x = static_cast<int>(position.x - 1.0f); // halfWidth = 1.0
		rect.y = static_cast<int>(position.y - 1.0f); // halfHeight = 1.0
		rect.w = 2;  // width = 2 units (pixels)
		rect.h = 2;  // height = 2 units (pixels)
		// Set draw color (e.g. white)
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);

		// Optionally present renderer here or after the loop
		SDL_RenderPresent(renderer);
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
	if (!(floatToIntVec.x < GlobalVariables::worldChunkWidth && floatToIntVec.x >= 0 && floatToIntVec.y < GlobalVariables::worldChunkWidth && floatToIntVec.y >= 0)) {
		return emptyChunk;
	}
	return worldVecStore[floatToIntVec];
}


void WorldGeneration::forceUpdatePixelFromGlobal(const Vector2D<int>& position) {
	Vector2D<int> chunkCoord(0, 0);
	Vector2D<int> localCoord(0, 0);

	chunkCoord.x = position.x / GlobalVariables::chunkSize;
	chunkCoord.y = position.y / GlobalVariables::chunkSize;

	localCoord.x = (position.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;
	localCoord.y = (position.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;

	if (chunkCoord.x < GlobalVariables::worldChunkWidth && chunkCoord.x >= 0 && chunkCoord.y < GlobalVariables::worldChunkWidth && chunkCoord.y >= 0) {
		worldVecStore[chunkCoord].getDirtyRect().expand(localCoord.x, localCoord.y);
	}
}


Pixel*& WorldGeneration::getPixelFromGlobal(const Vector2D<int>& position) {
	Vector2D<int> chunkCoord(0, 0);
	Vector2D<int> localCoord(0, 0);

	chunkCoord.x = position.x / GlobalVariables::chunkSize;
	chunkCoord.y = position.y / GlobalVariables::chunkSize;

	localCoord.x = (position.x % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;
	localCoord.y = (position.y % GlobalVariables::chunkSize + GlobalVariables::chunkSize) % GlobalVariables::chunkSize;

	if (chunkCoord.x < GlobalVariables::worldChunkWidth && chunkCoord.x >= 0 && chunkCoord.y < GlobalVariables::worldChunkWidth && chunkCoord.y >= 0) {
		return worldVecStore[chunkCoord][localCoord.y][localCoord.x];
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
void WorldGeneration::updateNeighbors(const Vector2D<int>& globalPos, const Vector2D<int>& dir) {
	if (!(globalPos.x < GlobalVariables::worldChunkWidth && globalPos.x >= 0 && globalPos.y < GlobalVariables::worldChunkWidth && globalPos.y >= 0)) {
		return;
	}
	auto [neighborChunkCoord, neighborLocalCoord] = getCoords(globalPos + dir);
	worldVecStore[neighborChunkCoord].getDirtyRect().expand(neighborLocalCoord.x, neighborLocalCoord.y);
};

std::pair<Vector2D<int>, Vector2D<int>> WorldGeneration::getCoords(const Vector2D<int>& globalPos) {
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

void WorldGeneration::swapTwoValues(Vector2D<int> pos1, Vector2D<int> pos2) {
	auto [chunkCoord1, localCoord1] = getCoords(pos1);
	auto [chunkCoord2, localCoord2] = getCoords(pos2);
	if (!(chunkCoord1.x < GlobalVariables::worldChunkWidth && chunkCoord1.x >= 0 && chunkCoord1.y < GlobalVariables::worldChunkWidth && chunkCoord1.y >= 0)) {
		return;
	}
	if (!(chunkCoord2.x < GlobalVariables::worldChunkWidth && chunkCoord2.x >= 0 && chunkCoord2.y < GlobalVariables::worldChunkWidth && chunkCoord2.y >= 0)) {
		return;
	}
	Chunk& ch1 = worldVecStore[chunkCoord1];
	Chunk& ch2 = worldVecStore[chunkCoord2];

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
	if ((localCoord1.x == 0 || localCoord1.x == GlobalVariables::chunkSize - 1 ||
		localCoord1.y == 0 || localCoord1.y == GlobalVariables::chunkSize - 1)) {
		if (localCoord1.x == 0)
			updateNeighbors(pos1, { -1, 0 });
		else if (localCoord1.x == GlobalVariables::chunkSize - 1)
			updateNeighbors(pos1, { 1, 0 });

		if (localCoord1.y == 0)
			updateNeighbors(pos1, { 0, -1 });
		else if (localCoord1.y == GlobalVariables::chunkSize - 1)
			updateNeighbors(pos1, { 0, 1 });
	}

	if (!(localCoord2.x == 0 || localCoord2.x == GlobalVariables::chunkSize - 1 ||
		localCoord2.y == 0 || localCoord2.y == GlobalVariables::chunkSize - 1)) return;

	if (localCoord2.x == 0)
		updateNeighbors(pos2, { -1, 0 });
	else if (localCoord2.x == GlobalVariables::chunkSize - 1)
		updateNeighbors(pos2, { 1, 0 });

	if (localCoord2.y == 0)
		updateNeighbors(pos2, { 0, -1 });
	else if (localCoord2.y == GlobalVariables::chunkSize - 1)
		updateNeighbors(pos2, { 0, 1 });
}

void WorldGeneration::burntSmoke(const int row, const int col) {
	Pixel*& pixelRef = getPixelFromGlobal(Vector2D(col, row));
	delete pixelRef;
	pixelRef = smoke->clone();
	pixelRef->setProcessed(true);
}