#ifndef VELOCITY
#define VELOCITY
#include <iostream>
#include <tuple>
#include <cmath>

// Vector2D is now a class template
template <typename T>
struct Vector2D {
	Vector2D(T xp, T yp) {
		x = xp;
		y = yp;
	}
	T x{};
	T y{};

	bool operator<(const Vector2D<T>& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}

	bool operator==(const Vector2D<T>& other) const {
		return (x == other.x) && (y == other.y);
	}
};

class Velocity {
public:
	Velocity();
	~Velocity();

	Vector2D<float> getVelocity() {
		return velocity;
	};
	void velocityTick();
	void resetVelocity() {
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	void setVelocity(int pX, int pY) {
		velocity.x = (float)pX;
		velocity.y = (float)pY;
	}

	void addForce(float vectorLength, float angle);
	void setIsGrounded(bool value) {
		isGrounded = value;
	}

private:
	Vector2D<float> velocity;
	const float gravity{ 1.1f };
	const float mass{ 1 };

	const float maxYVelocity{ 8.5f };
	const float maxXVelocity{ 6.5f };

	bool isGrounded{ false };

};

#endif /* VELOCITY */
