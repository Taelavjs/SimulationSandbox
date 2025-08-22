#include "Oil.hpp"
#include <random>

Oil::Oil(const PixelType* type) : Liquid(type) {}
Pixel* Oil::clone() const {
	return new Oil(type);
}
void Oil::update(int row, int col, WorldGeneration& worldGeneration) {
	Liquid::update(row, col, worldGeneration);
}