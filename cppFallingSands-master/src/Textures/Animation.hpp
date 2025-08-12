#ifndef ANIMATION
#define ANIMATION
#include "SDL.h"
#include "SDL_image.h"
#include <string>
#include <vector>

class Animation {
public:
	Animation(std::string iTitle, std::vector<SDL_Rect> iFrameReel, SDL_Texture* iTexture, int fps, bool isLoop);
	Animation();
	~Animation();
	SDL_Rect* play();
	void resetCounter();

private:
	void updateFrame();
	std::string title{ "" };
	int animationRate{ 0 };
	int currentFrame{ 0 };
	int totalFrames{ 0 };
	double deltaTime{ 0.0f };
	double prevTime{ 0.0f };
	SDL_Texture* texture;
	bool loop;
	std::vector<SDL_Rect> frameRects;
};

#endif /* ANIMATION */
