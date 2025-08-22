#include "Sand.hpp"

Sand::Sand(const PixelType* type) : SolidDynamic(type) {}

Pixel* Sand::clone() const {
	return new Sand(type);
}
