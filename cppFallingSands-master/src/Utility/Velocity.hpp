#ifndef VECTOR2D
#define VECTOR2D
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

	Vector2D operator+(const Vector2D<T>& other) const {
		return Vector2D<T>{ x + other.x, y + other.y };
	}

	bool operator!=(const Vector2D<T>& other) const {
		return  !((x == other.x) && (y == other.y));
	}

};
namespace std {
	template<typename T>
	struct hash<Vector2D<T>> {
		size_t operator()(const Vector2D<T>& v) const {
			size_t h1 = hash<T>{}(v.x);
			size_t h2 = hash<T>{}(v.y);
			return h1 ^ (h2 << 1);
		}
	};
}
#endif

#ifndef VELOCITY
#define VELOCITY
class Velocity {
public:
	Velocity();
	~Velocity();

	Vector2D<float> getVelocity();
	void velocityTick();
	void resetVelocity();

	void setVelocity(int pX, int pY);

	void addForce(float vectorLength, float angle);
	void setIsGrounded(bool value);

private:
	Vector2D<float> velocity;
	const float gravity{ 1.1f };
	const float mass{ 1 };

	const float maxYVelocity{ 8.5f };
	const float maxXVelocity{ 6.5f };

	bool isGrounded{ false };

};

#endif /* VELOCITY */
