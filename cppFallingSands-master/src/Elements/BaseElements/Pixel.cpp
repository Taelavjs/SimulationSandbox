#include "Pixel.hpp"
#include <algorithm>
#include "../../Utility/GlobalVariables.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <random>
#include <vector>
Pixel::Pixel() {}
Pixel::~Pixel() {}

// FIRE SECTIONß
void Pixel::ignite() {
	if (getOnFire()) return; // already ignited
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	if (dis(gen) > chanceToIgnite) {
		setOnFire();
	}
}
bool Pixel::hit() {
	hp--;
	return hp <= 0;
}

bool Pixel::fireTick(WorldGeneration& worldGeneration, const int& row, const int& col) {
	if (getOnFire()) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				// Archaic solution to not check diagonally for any flammable pixels
				// Simple comparisons so should be optimised
				// Not Ideal
				if (j == 0 && i == 0) continue;
				if (j == 1 && (i == 1 || i == -1)) continue;
				if (j == -1 && (i == 1 || i == -1)) continue;
				if (i == 1 && (j == 1 || j == -1)) continue;
				if (i == -1 && (j == 1 || j == -1)) continue;

				// Boundary Checks
				if (row + i > (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) - 1 || row + i < 0 || col + j >(GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) - 1 || col + j < 0) continue;

				// Grab necessary pixel from world space
				Pixel* ptrToNeighbor = worldGeneration.getPixelFromGlobal(Vector2D(col + i, row + j));
				if (ptrToNeighbor != nullptr && ptrToNeighbor->getIsFlammable() && !ptrToNeighbor->getOnFire()) {
					ptrToNeighbor->ignite();
				}
				else if (ptrToNeighbor != nullptr && ptrToNeighbor->getIsLiquid() && !ptrToNeighbor->getIsFlammable()) {
					// Hit returns true when the pixel has died of hp loss from being burnt
					if (ptrToNeighbor->hit()) {
						worldGeneration.burntSmoke(row + j, col + i);
					}
				}
			}
		}

		// If this pixel, which is on fire, dies, it returns true and gets deleted itself
		return hit();
	}
	return false;
}


// Group 1
bool Pixel::getIsMoveable() { return isMoveable; }
bool Pixel::getIsLiquid() { return isLiquid; }
bool Pixel::getIsSolid() { return isSolid; }
bool Pixel::getIsGas() { return isGas; }
bool Pixel::getIsFlammable() { return isFlammable; }
//

	// SETTERS ELEMENTAL FEATURES
void Pixel::setIsMoveable(bool val) { isMoveable = val; }
void Pixel::setIsLiquid(bool val) { isLiquid = val; }
void Pixel::setIsSolid(bool val) { isSolid = val; }
void Pixel::setIsGas(bool val) { isGas = val; }
void Pixel::setIsFlammable(bool val) { isFlammable = val; }

// GETTERS FUNCTIONS
bool Pixel::getProcessed() { return isProcessed; }
int Pixel::getMass() { return mass; }
uint32_t Pixel::getColour() { return color; }
double Pixel::getChanceToIgnite() { return chanceToIgnite; }
int Pixel::getHp() { return hp; }
int Pixel::getDensity() { return density; }
int Pixel::getTerminalX() { return terminalX; }
int Pixel::getTerminalY() { return terminalY; }
bool Pixel::getOnFire() { return onFire; };

// SETTERS FUNCTIONS
void Pixel::setProcessed(bool val) { isProcessed = val; }
void Pixel::setFlammable(bool val) { isFlammable = val; }
void Pixel::setHp(int health) { hp = health; }
void Pixel::setMass(int val) { mass = val; }
void Pixel::setChanceToIgnite(double val) { chanceToIgnite = val; }
void Pixel::setOnFire() { onFire = true; };
void Pixel::setDensity(int val) { density = val; }
void Pixel::setColor(uint32_t val) { color = val; }