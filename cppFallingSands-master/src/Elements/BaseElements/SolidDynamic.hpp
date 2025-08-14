#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP

#include "Moveable.hpp"
class WorldGeneration;

class SolidDynamic : public Moveable {
public:

protected:
	SolidDynamic();
	virtual ~SolidDynamic();
protected:
	virtual void update(int row, int col, const int& vecWidth, const int& vecHeight, WorldGeneration& worldGeneration);
	virtual void xDisperse(Chunk& vec, int row, int col, int xDispersion, int xDirection, int& res);
	bool checkMoveableMat(Pixel* space);
	int x_direction{ 0 };
	int yVelocity{ 1 };
	int xVelocity{ 1 };
};


#endif /* SOLIDDYNAMIC_HPP */
