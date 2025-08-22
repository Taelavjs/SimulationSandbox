#include "SolidImmoveable.hpp"

SolidImmoveable::SolidImmoveable(const PixelType* type) : Pixel(type) {}
void SolidImmoveable::update(int row, int col, WorldGeneration& worldGeneration) {
}
Pixel* SolidImmoveable::clone() const {
	return new SolidImmoveable(type);
}
