#include "SolidDynamic.hpp"
#include "../../Utility/GlobalVariables.hpp"
#include <random>

SolidDynamic::SolidDynamic(const PixelType* type) : Moveable(type) {
}

SolidDynamic::~SolidDynamic() {}

void SolidDynamic::update(int row, int col, WorldGeneration& worldGeneration) {
	setProcessed(true);
	const int maxCount = 3;
	int count = 0;

	while (count < maxCount) {
		Pixel*& pixelBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1));
		if (pixelBelow == nullptr || checkMoveableMat(pixelBelow)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
			row++;
			count++;
		}
		else {
			break;
		}
	}

	if (count > 0) return;
	x_direction = -1;
	for (int i = 0; i < 2; ++i) {
		Pixel*& diagonalPixel = worldGeneration.getPixelFromGlobal(Vector2D(col + x_direction, row + 1));
		if (diagonalPixel == nullptr || checkMoveableMat(diagonalPixel)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row + 1));
			x_direction *= -1;
			col += x_direction;
		}
	}
}

bool SolidDynamic::checkMoveableMat(Pixel* space) {
	return space->type->isMoveable && (space->type->isLiquid || space->type->isGas);
}

