#include "Oil.hpp"

Oil::Oil()
{
	setMass(14);
	setFlammable(true);
	setHp(55);
	setDensity(10);
}

Oil::~Oil() {}

uint32_t Oil::getColour() {
	if (getOnFire()) {
		return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 109, 57, 55, 255);

	}
	return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 40, 30, 21, 255);
}

Pixel* Oil::clone() const {
	return new Oil();
}