#ifndef VELOCITY
#define VELOCITY
#include <iostream>
#include <tuple>
#include <cmath>

struct Vector2D {
    Vector2D(float xp, float yp){
        x = xp;
        y = yp;
    }
    float x{0};
    float y{0};

    bool operator<(const Vector2D& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Vector2D& other) const {
        return (x == other.x) && (y == other.y);
    }
};

class Velocity {
public:
    Velocity();
    ~Velocity();

    Vector2D getVelocity(){
        return velocity;
    };
    void velocityTick();
    void resetVelocity(){
        velocity.x = 0;
        velocity.y = 0;
    }

    void setVelocity(int pX, int pY){
        velocity.x = pX;
        velocity.y = pY;
    }

    void addForce(float vectorLength, float angle);
    void setIsGrounded(bool value){
        isGrounded = value;
    }

private:
    Vector2D velocity;
    const float gravity{1.1f};
    const float mass{1};

    const float maxYVelocity{8.5f};
    const float maxXVelocity{6.5f};

    bool isGrounded{false};

};

#endif /* VELOCITY */
