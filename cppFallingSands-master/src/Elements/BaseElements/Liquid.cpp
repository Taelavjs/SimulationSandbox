#include "Liquid.hpp"
#include <algorithm>


Liquid::Liquid() {
	setIsMoveable(true);
	setIsLiquid(true);
}
Liquid::~Liquid() {}

int Liquid::getMovingDirection() {
	return x_direction;
}

void Liquid::update(int row, int col, const int& vecWidth, const int& vecHeight, WorldGeneration& worldGeneration)
{
	setProcessed(true);

	if (!getOnFire() && getHp() < maxHp) {
		setHp(getHp() + 1);
	}

	bool moved = true;
	int blocksFallen = 0;
	while (moved && blocksFallen <= yVelocity) {
		moved = false; // Assume no movement; change if a swap is made

		Pixel* pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row + 1));
		if (row + 1 < vecHeight * 2 && pixBelow == nullptr) {
			// Move down if the space below is empty
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
			row += 1; // Update row after moving
			x_direction = 0;
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else if (row + 1 < vecHeight * 2 && pixBelow != nullptr && pixBelow->getIsLiquid() && pixBelow->getDensity() < getDensity()) {
			// Move down if the space below has a less dense liquid
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row + 1));
			row += 1; // Update row after moving
			x_direction = 0;
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else {
			// If no valid move down is possible, break the loop
			break;
		}
	}

	// No lateral movement checks; pixel can only move left or right in a separate update
	if (blocksFallen > 0) {
		yVelocity += 1;
		yVelocity = std::min(yVelocity, 2);
		return;
	};
	yVelocity = 1;

	blocksFallen = 0;
	moved = true;
	while (moved && blocksFallen <= xVelocity) {
		moved = false; // Assume no movement; change if a swap is made

		Pixel* leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
		Pixel* rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

		bool isLeftValid = col - 1 >= 0 && (leftPix == nullptr || (leftPix->getIsLiquid() && leftPix->getDensity() < getDensity()));
		bool isRightValid = col + 1 < vecHeight * 2 && (rightPix == nullptr || (rightPix->getIsLiquid() && rightPix->getDensity() < getDensity()));

		if (isLeftValid && isRightValid) {
			// Randomly decide to move left or right if both directions are valid
			x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
			col += x_direction; // Update column after moving
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else if (isLeftValid) {
			// Move left if only left is valid
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
			col -= 1; // Update column after moving
			x_direction = -1;
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else if (isRightValid) {
			// Move right if only right is valid
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
			col += 1; // Update column after moving
			x_direction = 1;
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else {
			// No valid horizontal move found, break the loop
			break;
		}
	}

	if (blocksFallen > 0) {
		xVelocity += 1;
		xVelocity = std::min(xVelocity, 3);
		return;
	};

	xVelocity = 1;
}
