#include "Smoke.hpp"
#include "BaseElements/Pixel.hpp"
#include "BaseElements/Gas.hpp"

Smoke::Smoke()
{
	setIsGas(true);
	setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), (Uint8)130.0f + randomNumber(), (Uint8)130.0f + randomNumber(), 130, 255));
}

Smoke::~Smoke() {}
