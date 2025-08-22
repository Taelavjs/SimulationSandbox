#include "Pixel.hpp"
#include <algorithm>
#include "../../Utility/GlobalVariables.hpp"
#include <iostream>
#include <random>
#include <vector>

// Constructor now takes a PixelType pointer
Pixel::Pixel(const PixelType* type) : type(type) {}

// Redirecting getters to the PixelType object
uint32_t Pixel::getColour() const { return type->color; }
int Pixel::getMass() const { return type->mass; }
double Pixel::getChanceToIgnite() const { return type->chanceToIgnite; }
int Pixel::getDensity() const { return type->density; }
bool Pixel::getIsFlammable() const { return type->isFlammable; }
bool Pixel::getIsLiquid() const { return type->isLiquid; }

void Pixel::ignite() {
	if (getOnFire()) return;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	if (dis(gen) > type->chanceToIgnite) {
		setOnFire();
	}
}

bool Pixel::hit() {
	hp--;
	return hp <= 0;
}

bool Pixel::fireTick(WorldGeneration& worldGeneration, const int& row, const int& col) {
	if (!getOnFire()) {
		return false;
	}

	const int neighborCoords[4][2] = {
		{0, 1},
		{0, -1},
		{-1, 0},
		{1, 0}
	};

	for (int i = 0; i < 4; ++i) {
		int neighborRow = row + neighborCoords[i][0];
		int neighborCol = col + neighborCoords[i][1];

		// Boundary check (assuming global dimensions exist)
		if (neighborRow < 0 || neighborRow >= (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) ||
			neighborCol < 0 || neighborCol >= (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth)) {
			continue;
		}

		Pixel*& ptrToNeighbor = worldGeneration.getPixelFromGlobal(Vector2D(neighborCol, neighborRow));

		if (ptrToNeighbor != nullptr) {
			// Access static properties via `type` pointer and dynamic state directly
			if (ptrToNeighbor->getIsFlammable() && !ptrToNeighbor->getOnFire()) {
				ptrToNeighbor->ignite();
			}
			else if (ptrToNeighbor->getIsLiquid() && !ptrToNeighbor->getIsFlammable()) {
				if (ptrToNeighbor->hit()) {
					worldGeneration.burntSmoke(neighborRow, neighborCol);
				}
			}
		}
	}
	return hit();
}