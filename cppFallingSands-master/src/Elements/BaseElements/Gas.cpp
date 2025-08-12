#include "Gas.hpp"

#include "Gas.hpp"

Gas::Gas()
{
	setMass(300);
	setIsMoveable(true);
	setIsGas(true);
}

Gas::~Gas()
{

}

bool Gas::isGas()
{
	return true;
};

void Gas::update(int row, int col, const int& vecWidth, const int& vecHeight, WorldGeneration& worldGeneration)
{
	setProcessed(true);

	bool moved = true;
	int blocksFallen = 0;
	while (moved && blocksFallen <= yVelocity) {
		moved = false;

		// Awful lot of conversions when positional coordinates are always int
		// Create a vector for positional chords as ints? 
		Pixel*& pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row - 1));
		if (row - 1 >= 0 && pixBelow == nullptr) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
			row -= 1;
			x_direction = 0;
			moved = true;
			blocksFallen++;
		}
		else if (row - 1 >= 0 && pixBelow != nullptr && (pixBelow->getIsMoveable()) && !pixBelow->getIsGas()) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
			row -= 1; // Update row after moving
			x_direction = 0;
			moved = true; // Indicate a move was made
			blocksFallen++;
		}
		else {
			// If no valid move down is possible, break the loop
			break;
		}
	}

	if (blocksFallen > 0) {
		yVelocity += 2;
		yVelocity = std::min(yVelocity, 4);
		return;
	};
	yVelocity = 1;

	blocksFallen = 0;
	moved = true;
	while (moved && blocksFallen <= 4) {
		moved = false;

		Pixel* leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
		Pixel* rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

		bool isLeftValid = col - 1 >= 0 && (leftPix == nullptr || (leftPix->getIsMoveable() && !leftPix->getIsGas() && !leftPix->getIsSolid()));
		bool isRightValid = col + 1 < vecHeight * 2 && (rightPix == nullptr || (rightPix->getIsMoveable() && !rightPix->getIsGas() && !rightPix->getIsSolid()));

		if (isLeftValid && isRightValid) {
			x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
			col += x_direction;
			moved = true;
			blocksFallen++;
		}
		else if (isLeftValid) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
			col -= 1;
			x_direction = -1;
			moved = true;
			blocksFallen++;
		}
		else if (isRightValid) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
			col += 1;
			x_direction = 1;
			moved = true;
			blocksFallen++;
		}
		else {
			break;
		}
	}

	if (blocksFallen > 0) {
		return;
	};

}
