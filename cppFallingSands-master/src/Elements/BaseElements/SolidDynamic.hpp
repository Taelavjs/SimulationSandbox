#ifndef SOLIDDYNAMIC_HPP
#define SOLIDDYNAMIC_HPP

#include "Moveable.hpp"
class WorldGeneration;

class SolidDynamic : public Moveable {
public:
	SolidDynamic(const PixelType* type);
	virtual ~SolidDynamic();
protected:
	virtual void update(int row, int col, WorldGeneration& worldGeneration) override;

	void xDisperse(int row, int col, int xDispersion, int xDirection, WorldGeneration& worldGeneration, int& res);

	bool checkMoveableMat(Pixel* space);

private:
	int x_direction{ 0 };
};

#endif /* SOLIDDYNAMIC_HPP */