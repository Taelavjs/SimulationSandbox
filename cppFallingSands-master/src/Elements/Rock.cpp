#include "Rock.hpp"

Rock::Rock(const PixelType* type) : SolidImmoveable(type) {}

Pixel* Rock::clone() const {
	return new Rock(type);
}
