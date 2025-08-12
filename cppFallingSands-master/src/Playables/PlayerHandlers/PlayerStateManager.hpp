#ifndef PLAYERSTATEMANAGER
#define PLAYERSTATEMANAGER
#include "../../Utility/Velocity.hpp"
enum playerStates {
	Idle,
	Running,
	Jumping,
	Falling
};

class PlayerStateManager {
public:
	PlayerStateManager();
	~PlayerStateManager();

private:
	playerStates currentState{ playerStates::Idle };
	void idleStateChangeConditions(Vector2D<float> velocity, bool isGrounded);
	void runningStateChangeConditions(Vector2D<float> velocity, bool isGrounded);
	void jumpingStateChangeConditions(Vector2D<float> velocity, bool isGrounded);
	void fallingStateChangeConditions(Vector2D<float> velocity, bool isGrounded);

public:
	playerStates getCurrentState();
	void updatePlayerState(Vector2D<float> playerVelocity, bool isGrounded);

};
#endif /* PLAYERSTATEMANAGER */
