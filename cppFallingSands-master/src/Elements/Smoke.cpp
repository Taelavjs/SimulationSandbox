#include "Smoke.hpp"
#include <random>

Smoke::Smoke(const PixelType* type) : Gas(type) {}

Pixel* Smoke::clone() const {
	return new Smoke(type);
}
