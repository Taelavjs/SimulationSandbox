#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP
#include "Pixel.hpp"

class Moveable : public Pixel {
public:
	Moveable();
	~Moveable();
	float randomNumber();
	void updateVelocity(int& newCol, int upDown);
	void resetVelocity();
	bool isSpaceFree(Chunk& vec, int row, int col);
private:
	int yVelocity{ 0 };
	int xVelocity{ 0 };
	float verticalToHorizontalRation{ 0.4f };
};

#endif /* MOVEABLE_HPP */
