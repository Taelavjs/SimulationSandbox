#include "Gas.hpp"
#include <cmath>
#include <random>
#include <iostream>
#include "../../Utility/GlobalVariables.hpp"
#include <algorithm>

Gas::Gas(const PixelType* type) : Moveable(type) {}

void Gas::update(int row, int col, WorldGeneration& worldGeneration) {
	setProcessed(true);
	const int maxMove = 4;
	int moveCount = 0;
	bool moved = false;

	while (moveCount < maxMove) {
		Pixel*& pixBelow = worldGeneration.getPixelFromGlobal(Vector2D(col, row - 1));
		if (row - 1 >= 0 && pixBelow == nullptr) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
			row -= 1;
			x_direction = 0;
			moved = true;
			moveCount++;
		}
		else if (row - 1 >= 0 && pixBelow != nullptr && (pixBelow->type->isMoveable) && !pixBelow->type->isGas) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col, row - 1));
			row -= 1;
			x_direction = 0;
			moved = true;
			moveCount++;
		}
		else {
			break;
		}
	}


	if (moved) {
		return;
	};


	while (moveCount < maxMove) {
		Pixel*& leftPix = worldGeneration.getPixelFromGlobal(Vector2D(col - 1, row));
		Pixel*& rightPix = worldGeneration.getPixelFromGlobal(Vector2D(col + 1, row));

		bool isLeftValid = col - 1 >= 0 && (leftPix == nullptr || (leftPix->type->isMoveable && !leftPix->type->isGas && !leftPix->type->isSolid));
		bool isRightValid = col + 1 < GlobalVariables::worldChunkWidth * GlobalVariables::chunkSize && (rightPix == nullptr || (rightPix->type->isMoveable && !rightPix->type->isGas && !rightPix->type->isSolid));

		if (isLeftValid && isRightValid && x_direction == 0) {
			x_direction = x_direction == 0 ? (rand() % 2 == 0 ? -1 : 1) : x_direction;
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + x_direction, row));
			col += x_direction;
			moveCount++;
		}
		else if (isLeftValid && (x_direction == -1 || x_direction == 0)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col - 1, row));
			col -= 1;
			x_direction = -1;
			moveCount++;
		}
		else if (isRightValid && (x_direction == -1 || x_direction == 0)) {
			worldGeneration.swapTwoValues(Vector2D(col, row), Vector2D(col + 1, row));
			col += 1;
			x_direction = 1;
			moveCount++;
		}
		else {
			x_direction = 0;
			return;
		}
	}

	x_direction = 0;


}