#include "Velocity.hpp"
#include "GlobalVariables.hpp"
#include <SDL_stdinc.h>
Velocity::Velocity() :
	velocity(0.0f, 0.0f), mass(10)
{
}
Velocity::~Velocity() {

}
Vector2D<float> Velocity::getVelocity() {
	return velocity;
};
void Velocity::resetVelocity() {
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

void Velocity::setVelocity(int pX, int pY) {
	velocity.x = (float)pX;
	velocity.y = (float)pY;
}

void Velocity::setIsGrounded(bool value) {
	isGrounded = value;
}

void Velocity::velocityTick() {
	if (!isGrounded) {
		velocity.y -= gravity * mass;
		velocity.x *= 9.5f * GlobalVariables::getDeltaTime();
	}
	else {
		velocity.y = 0;
		velocity.x *= 6.0f * GlobalVariables::getDeltaTime();
	}
	if (velocity.y > 0) {
		velocity.y = std::min(velocity.y, maxYVelocity);

		if (velocity.y < 0.05) {
			velocity.y = 0;
		}
	}
	else if (velocity.y < 0) {
		velocity.y = std::max(velocity.y, -maxYVelocity);
		if (velocity.y > -0.05) {
			velocity.y = 0;
		}
	}
	if (velocity.x > 0) {
		velocity.x = std::min(velocity.x, maxXVelocity);
		if (velocity.x < 0.05) {
			velocity.x = 0;
		}
	}
	else if (velocity.x < 0) {
		velocity.x = std::max(velocity.x, -maxXVelocity);
		if (velocity.x > -0.05) {
			velocity.x = 0;
		}
	}
}

void Velocity::addForce(float vectorLength, float angle) {
	float radianAngle = (float)angle * (M_PI / 180.0f);
	velocity.x += std::cos(radianAngle) * vectorLength * GlobalVariables::getDeltaTime();
	velocity.y += std::sin(radianAngle) * vectorLength * GlobalVariables::getDeltaTime();

}


