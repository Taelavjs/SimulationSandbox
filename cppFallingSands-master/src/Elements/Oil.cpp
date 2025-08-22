#include "Oil.hpp"
#include <random>

Oil::Oil(const PixelType* type) : Liquid(type) {}
Pixel* Oil::clone() const {
	return new Oil(type);
}
