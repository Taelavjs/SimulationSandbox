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
	void update(SDL_Renderer* renderer, WorldGeneration& worldGeneration);
	std::stack<SDL_Rect> getStackRender();

	void playerInputHandler(SDL_Scancode e);
	void playerReleaseHandler(SDL_Scancode e);

	Velocity velocity;

private:
	void checkAreaCollision(bool& isBlockInPlayer, std::vector<SDL_Rect>& collisions, WorldGeneration& worldGeneration);
	bool isFlipTexture();
	void handleCollision(SDL_Rect* colliderRect);
	void playerForcesInputs();
	void collisionHandler(WorldGeneration& worldGeneration);
	void resetPlayerColliders();

private:
	SDL_Rect playerAABB;
	SDL_Rect groundedRect;
	std::stack<SDL_Rect> stckToRender;
	Sprite* playerSprite;
	Vector2D<float> position;
	Vector2D<float> validPosition;
	Vector2D<float> playerCenterPosition;
	Vector2D<float> playerScale;
	bool dLeft{ false }, dRight{ false }, dUp{ false }, dDown{ false };
	bool isFlipped{ false };
	std::map<playerStates, Animation> animations;
	bool isGrounded{ false };
	PlayerStateManager stateManager;
	playerStates prev;

	float jumpDelay{ 0.5f };
	float lastTimeJumpedAccumulated{ 0.0f };
};

#endif /* PLAYER_HPP */
