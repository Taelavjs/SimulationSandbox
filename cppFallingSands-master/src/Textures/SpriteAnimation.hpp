#ifndef SPRITEANIMATION
#define SPRITEANIMATION
#include "SDL.h"
#include "SDL_image.h"
#include "Animation.hpp"
#include <string>
class SpriteAnimation {
public:
	SpriteAnimation() = delete;

	static Animation createAnimation(std::string title, std::vector<SDL_Rect> rects, SDL_Texture* texture, int noFrames, int fps, bool isLoop);

};

#endif /* SPRITEANIMATION */
