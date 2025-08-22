#include "Water.hpp"
#include <random>
Water::Water(const PixelType* type) : Liquid(type) {}

void Water::update(int row, int col, WorldGeneration& worldGeneration) {
	Liquid::update(row, col, worldGeneration);
}

Pixel* Water::clone() const {
	return new Water(type);
}
