#include "Smoke.hpp"
#include <random>

Smoke::Smoke(const PixelType* type) : Gas(type) {}
void Smoke::update(int row, int col, WorldGeneration& worldGeneration) {
	Gas::update(row, col, worldGeneration);
}

Pixel* Smoke::clone() const {
	return new Smoke(type);
}
