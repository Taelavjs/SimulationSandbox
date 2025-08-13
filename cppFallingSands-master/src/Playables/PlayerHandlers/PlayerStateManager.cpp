#include "PlayerStateManager.hpp"

PlayerStateManager::PlayerStateManager()
{

}

PlayerStateManager::~PlayerStateManager()
{

}

playerStates PlayerStateManager::getCurrentState()
{
	return currentState;
}

void PlayerStateManager::updatePlayerState(Vector2D<float> playerVelocity, bool isGrounded)
{
	switch (currentState) {
	case playerStates::Idle:
		//std::cout << "Current State : " << "Idle" << "Velocity : " << playerVelocity.x << " " << playerVelocity.y << '\n';
		idleStateChangeConditions(playerVelocity, isGrounded);
		break;
	case playerStates::Jumping:
		//std::cout << "Current State : " << "Jumping" << "Velocity : " << playerVelocity.x << " " << playerVelocity.y << '\n';
		jumpingStateChangeConditions(playerVelocity, isGrounded);
		break;
	case playerStates::Falling:
		//		std::cout << "Current State : " << "Falling" << "Velocity : " << playerVelocity.x << " " << playerVelocity.y << '\n';
		fallingStateChangeConditions(playerVelocity, isGrounded);
		break;
	case playerStates::Running:
		//std::cout << "Current State : " << "Running" << "Velocity : " << playerVelocity.x << " " << playerVelocity.y << '\n';
		runningStateChangeConditions(playerVelocity, isGrounded);
		break;
	}

}

void PlayerStateManager::idleStateChangeConditions(Vector2D<float> velocity, bool isGrounded) {
	if (velocity.y != 0 && !isGrounded) // Cannot be idle, but cannot be grounded
	{
		if (velocity.y < 0) currentState = playerStates::Falling;
		if (velocity.y > 0) currentState = playerStates::Jumping;
	}
	else if ((std::abs(velocity.x) > 0 || std::abs(velocity.x) < 0) && isGrounded) // Has to be grounded 
	{
		if (velocity.x < 0) currentState = playerStates::Running;
		if (velocity.x > 0) currentState = playerStates::Running;
	}
}

void PlayerStateManager::runningStateChangeConditions(Vector2D<float> velocity, bool isGrounded)
{
	if (velocity.x == 0 && velocity.y == 0)
	{
		currentState = playerStates::Idle;
	}
	else if (velocity.y != 0 && !isGrounded) // Cannot be idle, but cannot be grounded
	{
		if (velocity.y < 0) currentState = playerStates::Falling;
		if (velocity.y > 0) currentState = playerStates::Jumping;
	}
}

void PlayerStateManager::jumpingStateChangeConditions(Vector2D<float> velocity, bool isGrounded)
{
	if (isGrounded)
	{
		if (velocity.x != 0) {
			currentState = playerStates::Running;
		}
		else {
			currentState = playerStates::Idle;
		}
	}
	else {
		if (velocity.y <= 0) {
			currentState = playerStates::Falling;
		}
	}
}

void PlayerStateManager::fallingStateChangeConditions(Vector2D<float> velocity, bool isGrounded)
{
	if (isGrounded)
	{
		if (velocity.x != 0) {
			currentState = playerStates::Running;
		}
		else {
			currentState = playerStates::Idle;
		}
	}
	else {
		if (velocity.y > 0) {
			currentState = playerStates::Jumping;
		}
	}
}



