#include "Sand.hpp"

Sand::Sand(const PixelType* type) : SolidDynamic(type) {}
void Sand::update(int row, int col, WorldGeneration& worldGeneration) {
	SolidDynamic::update(row, col, worldGeneration);
}

Pixel* Sand::clone() const {
	return new Sand(type);
}
