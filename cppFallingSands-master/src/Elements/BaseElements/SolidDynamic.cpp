#include "SolidDynamic.hpp"
#include "../../Utility/GlobalVariables.hpp"
#include <random>

SolidDynamic::SolidDynamic(const PixelType* type) : Moveable(type) {
}

SolidDynamic::~SolidDynamic() {}

void SolidDynamic::update(int row, int col, WorldGeneration& worldGeneration) {
	setProcessed(true);

	Pixel*& pixelBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1));
	if (pixelBelow == nullptr || checkMoveableMat(pixelBelow)) {
		worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
		return;
	}

	static std::mt19937 gen(std::random_device{}());
	static std::uniform_int_distribution<> dis(0, 1);
	int direction = (dis(gen) == 0) ? -1 : 1;

	for (int i = 0; i < 2; ++i) {
		Pixel*& diagonalPixel = worldGeneration.getPixelFromGlobal(Vector2D(col + direction, row + 1));
		if (diagonalPixel == nullptr || checkMoveableMat(diagonalPixel)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + direction, row + 1));
			return;
		}
		direction *= -1;
	}
}

bool SolidDynamic::checkMoveableMat(Pixel* space) {
	return space->type->isMoveable && space->type->isSolid == false;
}

