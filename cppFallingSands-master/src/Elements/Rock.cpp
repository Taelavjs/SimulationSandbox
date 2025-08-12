#include "Rock.hpp"

Rock::Rock(){
    setColor(SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 200, 220, 150, 255)); 
}

Rock::~Rock(){}
