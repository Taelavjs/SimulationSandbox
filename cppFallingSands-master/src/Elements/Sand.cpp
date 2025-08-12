#include "Sand.hpp"

Sand::Sand()
{
    setDensity(5);
    setMass(2);
    setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 200, 200, 0, 255));
}
Sand::~Sand() {}
