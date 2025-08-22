#include "Liquid.hpp"
#include <random>
#include "../../Utility/GlobalVariables.hpp"
#include <algorithm>

// Constructor now takes the PixelType pointer and passes it to the base class.
Liquid::Liquid(const PixelType* type) : Moveable(type) {}

// A virtual clone function is required for the factory pattern.
// It creates a new object of the same type and copies the PixelType pointer.
Pixel* Liquid::clone() const {
	return new Liquid(type);
}

void Liquid::update(int row, int col, WorldGeneration& worldGeneration) {
	setProcessed(true);
	const int maxCount = 3;
	int count = 0;
	if (!getOnFire() && getHp() < 3) {
		setHp(getHp() + 1);
	}

	while (maxCount > count) {
		Pixel*& pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1));
		if (row + 1 < GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth && pixBelow == nullptr) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
			row += 1;
			x_direction = 0;
			count++;
		}
		else if (row + 1 < GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth && (pixBelow->type->isGas || pixBelow != nullptr && pixBelow->type->isLiquid && pixBelow->type->density < type->density)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
			row += 1;
			x_direction = 0;
			count++;
		}
		else {
			break;
		}

	}
	if (count != 0) return;

	while (count < maxCount) {
		Pixel*& leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
		Pixel*& rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

		bool isLeftValid = col - 1 >= 0 && (leftPix == nullptr || (leftPix->type->isLiquid && leftPix->type->density < type->density));
		bool isRightValid = col + 1 < GlobalVariables::chunkSize * GlobalVariables::worldChunkWidth && (rightPix == nullptr || (rightPix->type->isLiquid && rightPix->type->density < type->density));

		if (isLeftValid && isRightValid) {
			// Randomly decide to move left or right if both directions are valid
			x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
			col += x_direction;
			count++;

		}
		else if (isLeftValid) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
			col -= 1;
			x_direction = -1;
			count++;

		}
		else if (isRightValid) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
			col += 1;
			x_direction = 1;
			count++;
		}
		else {
			return;
		}
	}



}