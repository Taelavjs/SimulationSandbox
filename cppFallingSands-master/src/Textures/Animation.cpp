#include "Animation.hpp"

Animation::Animation(std::string iTitle, std::vector<SDL_Rect> iFrameReel, SDL_Texture* iTexture, int fps, bool isLoop)
    :frameRects(iFrameReel), title(iTitle), texture(iTexture), animationRate(fps), loop(isLoop)
{
    totalFrames = frameRects.size();
}

Animation::Animation()
{

}

Animation::~Animation()
{

}

SDL_Rect* Animation::play(){
    updateFrame();
    return &frameRects[currentFrame];
}

void Animation::updateFrame() {
    uint32_t tick_time = SDL_GetTicks();
    deltaTime += tick_time - prevTime;   
    prevTime = tick_time;                 
    
    int frameDuration = 1000 / animationRate;  

    if (deltaTime >= frameDuration) {
        if (currentFrame >= totalFrames - 1) {
            if(loop)
                currentFrame = 0;
        } else {
            currentFrame++;
        }
        deltaTime = 0;
    }
}

void Animation::resetCounter(){
    currentFrame = 0;
    prevTime = SDL_GetTicks();
    deltaTime   = 0;
}

