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
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/draw_constrained_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Point Point;
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
		std::vector<std::tuple<K::Point_2, K::Point_2, K::Point_2>> allTriangles;

		for (const auto& group : connectedLineGroups) {
			std::vector<Vector2D<float>> simplified = Douglas::DouglasPeucker(group, 2.0f);
			if (simplified.size() < 3) continue;
			allSimplifiedPolylines.push_back(simplified);

			CDT cdt;
			for (size_t i = 1; i < simplified.size(); ++i) {
				Point p1(simplified[i - 1].x, simplified[i - 1].y);
				Point p2(simplified[i].x, simplified[i].y);
				cdt.insert_constraint(p1, p2);
			}

			// Optional: close the loop if needed
			if (simplified.front() != simplified.back()) {
				Point p1(simplified.back().x, simplified.back().y);
				Point p2(simplified.front().x, simplified.front().y);
				cdt.insert_constraint(p1, p2);
			}

			// Traverse triangles
			for (auto fit = cdt.finite_faces_begin(); fit != cdt.finite_faces_end(); ++fit) {
				if (cdt.is_infinite(fit)) continue;

				Point p0 = fit->vertex(0)->point();
				Point p1 = fit->vertex(1)->point();
				Point p2 = fit->vertex(2)->point();

				allTriangles.emplace_back(p0, p1, p2);
			}
		}
		currentChunk.drawSimplifiedPolygonsTexture(simplifiedPolyLines, allTriangles, renderer);
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