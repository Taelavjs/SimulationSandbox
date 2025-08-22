#include "Water.hpp"
#include <random>
Water::Water(const PixelType* type) : Liquid(type) {}

Pixel* Water::clone() const {
	return new Water(type);
}
