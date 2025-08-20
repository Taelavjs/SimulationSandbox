#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "../Textures/Sprite.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include "../Elements/BaseElements/Pixel.hpp"
#include "../Utility/Velocity.hpp"
#include "PlayerHandlers/PlayerStateManager.hpp"
#include "../Textures/Animation.hpp"
#include <stack>
#include <map>

class Sprite;
class Player {
public:
	Player(Sprite* sprite);
	~Player();

	Sprite* getSprite();
	const Vector2D<float>& getCoordinates();
	const Vector2D<float>& getDimensions();
	const SDL_Rect& getPlayerRect();
	void renderPlayer(SDL_Renderer* renderer, int screenWidth);
	void renderDebugRects(SDL_Renderer* renderer);

	void update(SDL_Renderer* renderer, WorldGeneration& worldGeneration);
	std::stack<SDL_Rect> getStackRender();

	void playerInputHandler(SDL_Scancode e);
	void playerReleaseHandler(SDL_Scancode e);

	Velocity velocity;

private:
	bool checkAreaCollision(WorldGeneration& worldGeneration);
	bool checkFeetCollision(WorldGeneration& worldGeneration);
	int highestFeetCollisionPoint(WorldGeneration& worldGeneration);
	SDL_Rect renderWorldToScreenCoords(SDL_Rect& rectToRender);
	bool isFlipTexture();
	void handleCollision(SDL_Rect* colliderRect);
	void playerForcesInputs();
	void collisionHandler(WorldGeneration& worldGeneration);
	void resetPlayerColliders();
	std::vector<SDL_Rect> collisions;

private:
	std::stack<SDL_Rect> stckToRender;
	Sprite* playerSprite;
	Vector2D<float> playerScale;
	bool dLeft{ false }, dRight{ false }, dUp{ false }, dDown{ false };
	bool isFlipped{ false };
	std::map<playerStates, Animation> animations;
	bool isGrounded{ false };
	PlayerStateManager stateManager;
	playerStates prev;

	float jumpDelay{ 0.5f };
	float lastTimeJumpedAccumulated{ 1.0f };

private:
	Vector2D<float> playerPosition{ 0.0f, -22.0f };
	Vector2D<float> lastValidPosition{ 0.0f, -22.0f };
	SDL_Rect playerWorldRect;
	SDL_Rect playerWorldFeetRect;
	SDL_Rect leftDirection;
	SDL_Rect rightDirection;
};

#endif /* PLAYER_HPP */
