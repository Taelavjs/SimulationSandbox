#include "Napalm.hpp"

Napalm::Napalm()
{
	setDensity(3);
	setMass(8);
	movingRight = (randomNumber() > 0.5f);
	setOnFire();
	setFlammable(true);
	setHp(60);
	setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 50, 25, 200));
}

Napalm::~Napalm() {}

Pixel* Napalm::clone() const
{
	return new Napalm();
}