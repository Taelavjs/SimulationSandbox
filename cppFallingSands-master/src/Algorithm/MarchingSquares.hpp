#include "../Elements/BaseElements/Pixel.hpp"
#include <array>
#include <bitset>
#include <stack>
#include "../Utility/Velocity.hpp"

#ifndef MARCHING
#define MARCHING

struct MarchingSquares {
	static std::stack<std::pair<Vector2D<float>, Vector2D<float>>> run(
		Chunk& chunk,
		Chunk*& rightChunk,
		Chunk*& downChunk,
		Chunk*& rightDownChunk) {

		std::array<std::array<int, GlobalVariables::chunkSize>, GlobalVariables::chunkSize> results{};

		for (int i = 0; i < GlobalVariables::chunkSize; i++) {
			for (int j = 0; j < GlobalVariables::chunkSize; j++) {
				Pixel* topLeft = chunk[i][j];

				Pixel* topRight = nullptr;
				if (j == GlobalVariables::chunkSize - 1 && rightChunk != nullptr) {
					topRight = (*rightChunk)[i][0];
				}
				else if (j < GlobalVariables::chunkSize - 1) {
					topRight = chunk[i][j + 1];
				}

				Pixel* bottomLeft = nullptr;
				if (i == GlobalVariables::chunkSize - 1 && downChunk != nullptr) {
					bottomLeft = (*downChunk)[0][j];
				}
				else if (i < GlobalVariables::chunkSize - 1) {
					bottomLeft = chunk[i + 1][j];
				}

				if (rightDownChunk == nullptr && i == 15 && j == 15) {
					int x = 0;
				}

				Pixel* bottomRight = nullptr;
				if (i == GlobalVariables::chunkSize - 1 && j == GlobalVariables::chunkSize - 1 && rightDownChunk != nullptr) {
					bottomRight = (*rightDownChunk)[0][0];
				}
				else if (i == GlobalVariables::chunkSize - 1 && downChunk != nullptr) {
					bottomRight = (*downChunk)[0][j + 1];
				}
				else if (j == GlobalVariables::chunkSize - 1 && rightChunk != nullptr) {
					bottomRight = (*rightChunk)[i][0];
				}
				else if (i < GlobalVariables::chunkSize - 1 && j < GlobalVariables::chunkSize - 1) {
					bottomRight = chunk[i + 1][j + 1];
				}

				int squareIndex = 0;
				if (topLeft != nullptr && !topLeft->getIsMoveable())       squareIndex |= 1;
				if (topRight != nullptr && !topRight->getIsMoveable())     squareIndex |= 2;
				if (bottomLeft != nullptr && !bottomLeft->getIsMoveable())   squareIndex |= 4;
				if (bottomRight != nullptr && !bottomRight->getIsMoveable())  squareIndex |= 8;

				if (i < GlobalVariables::chunkSize && j < GlobalVariables::chunkSize) {
					results[i][j] = squareIndex;
				}
			}
		}

		std::stack<std::pair<Vector2D<float>, Vector2D<float>>> lines;
		const float halfInc = 0.5f;

		for (int i = 0; i < GlobalVariables::chunkSize - 1; i++) {
			for (int j = 0; j < GlobalVariables::chunkSize - 1; j++) {
				// It's standard practice to use j for the x-coordinate and i for the y-coordinate.
				const float x = static_cast<float>(j);
				const float y = static_cast<float>(i);

				// Define the midpoints of the four edges of the current cell
				const Vector2D<float> top(x + halfInc, y);
				const Vector2D<float> bottom(x + halfInc, y + 1.0f);
				const Vector2D<float> left(x, y + halfInc);
				const Vector2D<float> right(x + 1.0f, y + halfInc);

				switch (results[i][j]) {
					// 1 solid corner
				case 1: lines.push({ left, top }); break;      // Top-Left
				case 2: lines.push({ top, right }); break;     // Top-Right
				case 4: lines.push({ left, bottom }); break;   // Bottom-Left
				case 8: lines.push({ right, bottom }); break;  // Bottom-Right

					// 2 solid corners (adjacent)
				case 3: lines.push({ left, right }); break;    // Top-Left & Top-Right
				case 12: lines.push({ left, right }); break;   // Bottom-Left & Bottom-Right
				case 5: lines.push({ top, bottom }); break;    // Top-Left & Bottom-Left
				case 10: lines.push({ top, bottom }); break;   // Top-Right & Bottom-Right

					// 2 solid corners (diagonal - ambiguous cases)
					// This solution draws two lines to avoid making assumptions about connectivity.
				case 6: // Top-Right & Bottom-Left
					lines.push({ top, right });
					lines.push({ left, bottom });
					break;
				case 9: // Top-Left & Bottom-Right
					lines.push({ left, top });
					lines.push({ right, bottom });
					break;

					// 3 solid corners (equivalent to 1 empty corner)
				case 7: lines.push({ right, bottom }); break;  // All but Bottom-Right
				case 11: lines.push({ left, bottom }); break;  // All but Bottom-Left
				case 13: lines.push({ top, right }); break;    // All but Top-Right
				case 14: lines.push({ left, top }); break;     // All but Top-Left

					// 0 or 4 solid corners - no lines
				case 0:
				case 15:
				default:
					break;
				}
			}
		}
		return lines;
	}
};

#endif