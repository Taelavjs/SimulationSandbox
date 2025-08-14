#include "Water.hpp"
#include "BaseElements/Pixel.hpp"

Water::Water()
{
	setIsLiquid(true);
	setMass(8);
	movingRight = (randomNumber() > 0.5f);
	setHp(30);
	setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 0, 50, 180, 200));
}

Water::~Water() {}
Pixel* Water::clone() const { return new Water(); }
int Water::getDensity() const { return 5; };
