#ifndef LIQUID_HPP
#define LIQUID_HPP
#include "Pixel.hpp"
#include "Moveable.hpp"

class Liquid :public Moveable {
public:
	Liquid();
	~Liquid();
	bool swapOnDensity(Chunk& vec, int x1, int y1, int x2, int y2, int density1, int density2);
	int getDispersionRate() { return 5; };
	virtual void update(int row, int col, const int& vecWidth, const int& vecHeight, WorldGeneration& worldGeneration);
	int getMovingDirection();
private:
	int yVelocity{ 1 };
	int xVelocity{ 1 };
	int x_direction{ 0 };
	int density{ 5 };
	int maxHp{ 5 };
};


#endif /* LIQUID_HPP */
