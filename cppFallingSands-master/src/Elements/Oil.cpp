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
    if(getOnFire()){
        return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 150, 50, 30, 200);

    }
    return SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 90, 50, 30, 200);
}

