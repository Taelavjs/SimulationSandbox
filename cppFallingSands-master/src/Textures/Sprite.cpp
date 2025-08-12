#include "Sprite.hpp"

Sprite::Sprite(const char* srcPath) {
	SDL_Renderer* renderer = Rendering::getRenderer();
	surface = IMG_Load(srcPath);
	if (!surface) {
		std::cerr << "Failed to load surface: " << IMG_GetError() << std::endl;
		return;
	}

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
	}
}

Sprite::Sprite(const char* srcPath, int& width, int& height, int& rows, int& cols)
{
	SDL_Renderer* renderer = Rendering::getRenderer();
	rectSheet.resize(rows);
	for (int i = 0; i < rows; ++i) {
		rectSheet[i].resize(cols, { 0, 0, 0, 0 });
	}

	surface = IMG_Load(srcPath);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	for (int y = 0; y < rows; ++y) {
		for (int x = 0; x < cols; ++x) {

			SDL_Rect srcRect = { x * width, y * height, width, height };
			rectSheet[y][x] = srcRect;
		}
	}
}

void Sprite::showSpriteSheet(int& rows, int& cols) {
	// RENDERER DEBUG SETUP PRIOR TO INITIAL SETUP IN RENDERING CLASS
	SDL_Renderer* renderer = Rendering::getRenderer();
	SDL_SetRenderTarget(renderer, nullptr);  // Reset the rendering target after the loop
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	// ***

	int tileSize = 32;
	int spacing = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			SDL_Rect rect = {
				j * (tileSize + spacing),
				i * (tileSize + spacing),
				tileSize ,
				tileSize
			};
			SDL_RenderCopy(renderer, texture, &rectSheet[i][j], &rect);
		}
	}
	SDL_RenderPresent(renderer);
	SDL_PumpEvents();
	SDL_Delay(200);

	std::cout << "TextureSheet Loaded Correctly" << '\n';
}

Sprite::~Sprite() {
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

// This is flawed but works fine - sprite animation is based on fps not time currently
// This is not universally reusable
// SDL_Rect* Sprite::blinkCycle(){
//     if(rowTwoCounter >= 2) {
//         rowTwoCounter = 0;
//     }
//     rowTwoCounter++;
//     return &rectSheet[1][rowTwoCounter-1];
// }

// SDL_Rect* Sprite::runCycle(){
//     if(rowFourCounter >=5) {
//         rowFourCounter = 0;
//     }
//     return &rectSheet[3][rowFourCounter++];
// }

SDL_Texture* Sprite::getTexture() {
	return texture;
}

std::vector<std::vector<SDL_Rect>> Sprite::getRects() {
	return rectSheet;
}
