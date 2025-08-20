#include "Player.hpp"
#include "../Utility/GlobalVariables.hpp"
#include "../Textures/SpriteAnimation.hpp"
#include <iostream>
#include <string>
#include <tuple>





Player::Player(Sprite* sprite) :playerSprite(sprite), velocity(), stateManager(), playerScale(16, 16), playerWorldRect(), playerWorldFeetRect()
{
	SDL_Texture* texture = playerSprite->getTexture();
	prev = playerStates::Falling;

	animations[playerStates::Idle] = SpriteAnimation::createAnimation("Idle", playerSprite->getRects()[1], texture, 2, 2, true);
	animations[playerStates::Running] = SpriteAnimation::createAnimation("Running", playerSprite->getRects()[3], texture, 4, 18, true);
	animations[playerStates::Jumping] = SpriteAnimation::createAnimation("Jumping", playerSprite->getRects()[5], texture, 7, 14, false);
	animations[playerStates::Falling] = SpriteAnimation::createAnimation("Falling", playerSprite->getRects()[7], texture, 7, 8, false);


}

Player::~Player() {
	delete playerSprite;
}

Sprite* Player::getSprite() {
	return playerSprite;
}
const Vector2D<float>& Player::getCoordinates() {
	return playerPosition;
}
const Vector2D<float>& Player::getDimensions() {
	return playerScale;
}
const SDL_Rect& Player::getPlayerRect() { return playerWorldRect; }
std::stack<SDL_Rect> Player::getStackRender() { return stckToRender; };
void Player::playerReleaseHandler(SDL_Scancode e) {
	if (e == SDL_SCANCODE_RIGHT) {
		dRight = false;
	}
	if (e == SDL_SCANCODE_LEFT) {
		dLeft = false;
	}
	if (e == SDL_SCANCODE_UP) {
		dUp = false;
	}
	if (e == SDL_SCANCODE_DOWN) {
		dDown = false;
	}
}


void Player::playerInputHandler(SDL_Scancode e) {
	if (e == SDL_SCANCODE_RIGHT) {
		dRight = true;
	}
	if (e == SDL_SCANCODE_LEFT) {
		dLeft = true;
	}
	if (e == SDL_SCANCODE_UP) {
		dUp = true;
	}
	if (e == SDL_SCANCODE_DOWN) {
		dDown = true;
	}
}

void Player::playerForcesInputs() {
	if (dLeft) {
		velocity.addForce(80, 180);
	}
	if (dRight) {
		velocity.addForce(80, 0);
	}
	if (isGrounded && dUp && lastTimeJumpedAccumulated >= jumpDelay) {
		playerStates currentState = stateManager.getCurrentState();
		if ((currentState == playerStates::Idle || currentState == playerStates::Running)) {
			velocity.addForce(600, 90);
			lastTimeJumpedAccumulated = 0;
		}
	}
	lastTimeJumpedAccumulated += GlobalVariables::getDeltaTime();

}

void Player::resetPlayerColliders() {
	playerWorldRect.x = playerPosition.x;
	playerWorldRect.y = playerPosition.y + 4;
	playerWorldRect.w = 4;
	playerWorldRect.h = 15;

	playerWorldFeetRect.x = playerPosition.x - 2;
	playerWorldFeetRect.y = playerPosition.y + playerScale.y;
	playerWorldFeetRect.w = 8;
	playerWorldFeetRect.h = 6;

	leftDirection.x = playerWorldRect.x - 1;
	leftDirection.y = playerWorldRect.y + playerWorldRect.h - 8;
	leftDirection.w = 4;
	leftDirection.h = 8;

	rightDirection.x = playerWorldRect.x + playerWorldRect.w - 3;
	rightDirection.y = playerWorldRect.y + playerWorldRect.h - 8;
	rightDirection.w = 4;
	rightDirection.h = 8;
}

bool Player::checkAreaCollision(WorldGeneration& worldGeneration) {

	for (int i = playerPosition.x; i < playerPosition.x + playerWorldRect.w; i++) {
		if (i < 0) continue;
		for (int j = playerPosition.y; j < playerPosition.y + playerWorldRect.h + 1; j++) {
			if (j < 0) continue;
			Pixel* pix = worldGeneration.getPixelFromGlobal({ i, j });
			if (pix == nullptr) continue;
			if (pix->getIsSolid()) return true;
		}
	}
	return false;
}

bool Player::checkFeetCollision(WorldGeneration& worldGeneration) {

	for (int i = playerWorldFeetRect.x; i < playerWorldFeetRect.x + playerWorldFeetRect.w; i++) {
		if (i < 0) continue;
		for (int j = playerWorldFeetRect.y; j < playerWorldFeetRect.y + playerWorldFeetRect.h; j++) {
			if (j < 0) continue;

			Pixel* pix = worldGeneration.getPixelFromGlobal({ i, j });
			if (pix == nullptr) continue;
			if (pix->getIsSolid()) return true;
		}
	}
	return false;
}

int Player::highestFeetCollisionPoint(WorldGeneration& worldGeneration) {
	SDL_Rect checkRect = (velocity.getVelocity().x > 0) ? rightDirection : leftDirection;
	int highestSolidPixelY = 0;
	bool collided = false;

	for (int i = checkRect.x; i < checkRect.x + checkRect.w; ++i) {
		if (i < 0) continue;
		for (int j = checkRect.y; j < checkRect.y + checkRect.h; ++j) {
			if (j < 0) continue;
			Pixel* pix = worldGeneration.getPixelFromGlobal({ i, j });
			if (pix == nullptr) continue;
			if (pix->getIsSolid()) {
				collided = true;
				if (highestSolidPixelY == 0 || j < highestSolidPixelY) {
					highestSolidPixelY = j;
				}
			}
		}
	}

	return highestSolidPixelY;
}


void Player::collisionHandler(WorldGeneration& worldGeneration)
{
	if (checkFeetCollision(worldGeneration)) {
		isGrounded = true;
		velocity.setIsGrounded(true);
	}
	else {
		isGrounded = false;
		velocity.setIsGrounded(false);
	}
	lastValidPosition = playerPosition;
	int xVel = velocity.getVelocity().x;
	for (int tries = 0; tries < 6; tries++) {
		resetPlayerColliders();
		lastValidPosition = playerPosition;
		int highestStep = 0;
		if (std::abs(velocity.getVelocity().x) != 0) {
			highestStep = highestFeetCollisionPoint(worldGeneration);
			if (highestStep != 0) {
				playerPosition.y -= 1;
				resetPlayerColliders();
			}
		}
		playerPosition.x += xVel / 6.0f;
		if (checkAreaCollision(worldGeneration)) {
			playerPosition = lastValidPosition;

			velocity.setVelocity(0, velocity.getVelocity().y);
			break;
		}
	}
	resetPlayerColliders();
	int yVel = velocity.getVelocity().y;
	for (int tries = 0; tries < 6; tries++) {
		resetPlayerColliders();
		lastValidPosition = playerPosition;
		playerPosition.y -= yVel / 6.0f;
		if (checkAreaCollision(worldGeneration)) {
			playerPosition.y = lastValidPosition.y;
			velocity.setVelocity(velocity.getVelocity().x, 0);
			break;
		}
	}

}

void Player::update(SDL_Renderer* renderer, WorldGeneration& worldGeneration)
{
	velocity.velocityTick();
	playerForcesInputs();
	collisionHandler(worldGeneration);

	resetPlayerColliders();
	stateManager.updatePlayerState(velocity.getVelocity(), isGrounded);
}
SDL_Rect Player::renderWorldToScreenCoords(SDL_Rect& rectToRender) {
	const int camOffsetX = (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2 - playerPosition.x;
	const int camOffsetY = (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2 - playerPosition.y;
	SDL_Rect screenRect = {
		rectToRender.x + camOffsetX,
		rectToRender.y + camOffsetY,
		rectToRender.w,
		rectToRender.h
	};
	return screenRect;
}

void Player::renderPlayer(SDL_Renderer* renderer, int screenWidth) {
	if (prev != stateManager.getCurrentState()) {
		animations[prev].resetCounter();
		prev = stateManager.getCurrentState();
	}
	SDL_Texture* texture = playerSprite->getTexture();
	const SDL_Rect* rect = animations[stateManager.getCurrentState()].play();

	const SDL_Rect* dst = new SDL_Rect{ (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2, (GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth) / 2, (int)playerWorldRect.w, (int)playerWorldRect.h };
	SDL_RendererFlip flip;
	if (isFlipTexture()) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	else {
		flip = SDL_FLIP_NONE;
	}


	SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
	SDL_RenderCopyEx(renderer, texture, rect, dst, NULL, NULL, flip);
	renderDebugRects(renderer);
	delete dst;
}

void Player::renderDebugRects(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
	SDL_Rect a = renderWorldToScreenCoords(playerWorldRect);
	SDL_RenderDrawRect(renderer, &a);
	SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
	SDL_Rect b = renderWorldToScreenCoords(playerWorldFeetRect);
	SDL_RenderDrawRect(renderer, &b);
	SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
	SDL_Rect c = renderWorldToScreenCoords(rightDirection);
	SDL_RenderDrawRect(renderer, &c);
	SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);
	SDL_Rect d = renderWorldToScreenCoords(leftDirection);
	SDL_RenderDrawRect(renderer, &d);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void Player::handleCollision(SDL_Rect* colliderRect) {
}

bool Player::isFlipTexture() {
	Vector2D<float> playerVelocity = velocity.getVelocity();
	if (playerVelocity.x == 0) return isFlipped;
	if (playerVelocity.x < 0) {
		isFlipped = true;
	}
	else {
		isFlipped = false;
	}
	return isFlipped;
}

