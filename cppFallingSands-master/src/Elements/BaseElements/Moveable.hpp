#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP
#include "Pixel.hpp"
#include <math.h>
#include <random>


class Moveable : public Pixel {
public:
	Moveable();
	~Moveable();
	float randomNumber();
	void updateVelocity(int& newCol, int upDown);
	void resetVelocity() {
		yVelocity = 0;
	}
	bool isSpaceFree(Chunk& vec, int row, int col);
private:
	int yVelocity{ 0 };
	int xVelocity{ 0 };
	float verticalToHorizontalRation{ 0.4f };
};

#endif /* MOVEABLE_HPP */
