#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "SDL.h"
#include "SDL_image.h"
#include <vector>


class Sprite {
public:
	Sprite(const char* srcPath);
	Sprite(const char* srcPath, int& width, int& height, int& rows, int& cols);
	~Sprite();

	// GETTERS/SETTERS
	SDL_Texture* getTexture();
	std::vector<std::vector<SDL_Rect>> getRects();

	// DEBUG
	void showSpriteSheet(int& rows, int& cols);
private:
	SDL_Texture* texture;
	std::vector<std::vector<SDL_Rect>> rectSheet;
	SDL_Surface* surface;
};

#endif /* SPRITE_HPP */
