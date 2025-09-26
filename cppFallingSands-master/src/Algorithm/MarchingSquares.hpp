#include "../Elements/BaseElements/Pixel.hpp"
#include <array>
#include <bitset>
#include <stack>
#include "../Utility/Velocity.hpp"

#ifndef MARCHING
#define MARCHING

struct MarchingSquares {
	static std::stack<std::pair<Vector2D<float>, Vector2D<float>>> run(
		Chunk& chunk) {

		std::array<std::array<int, GlobalVariables::chunkSize + 1>, GlobalVariables::chunkSize + 1> results{};
		int chunkSize = GlobalVariables::chunkSize;

		for (int i = -1; i < chunkSize; i++) {
			for (int j = -1; j < chunkSize; j++) {

				auto getPixel = [&](int row, int col) -> Pixel* {
					if (row < 0 || row >= chunkSize || col < 0 || col >= chunkSize) {
						return nullptr; // Treat outside as empty space
					}
					return chunk[row][col];
					};
				Pixel* topLeft = getPixel(i, j);
				Pixel* topRight = getPixel(i, j + 1);
				Pixel* bottomLeft = getPixel(i + 1, j);
				Pixel* bottomRight = getPixel(i + 1, j + 1);

				int squareIndex = 0;

				if (topLeft != nullptr && !topLeft->type->isMoveable)     squareIndex += 1;

				if (topRight != nullptr && !topRight->type->isMoveable)   squareIndex += 2;

				if (bottomLeft != nullptr && !bottomLeft->type->isMoveable) squareIndex += 4;

				if (bottomRight != nullptr && !bottomRight->type->isMoveable) squareIndex += 8;

				results[i + 1][j + 1] = squareIndex;
			}
		}

		std::stack<std::pair<Vector2D<float>, Vector2D<float>>> lines;
		const float halfInc = 0.5f;

		for (int i = 0; i < GlobalVariables::chunkSize + 1; i++) {
			for (int j = 0; j < GlobalVariables::chunkSize + 1; j++) {
				float x = static_cast<float>(j);
				float y = static_cast<float>(i);
				x -= 1.0f;
				y -= 1.0f;

				const Vector2D<float> top(x + halfInc, y);
				const Vector2D<float> bottom(x + halfInc, y + 1.0f);
				const Vector2D<float> left(x, y + halfInc);
				const Vector2D<float> right(x + 1.0f, y + halfInc);

				switch (results[i][j]) {
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