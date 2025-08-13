#ifndef SUBCHUNKBOUNDINGBOX
#define SUBCHUNKBOUNDINGBOX
#include <algorithm> // For std::min and std::max
#include "../FallingSandsSumoSimulation/cppFallingSands-master/src/Utility/GlobalVariables.hpp"

class chunkBoundingBox {
public:
	chunkBoundingBox() : isDirty(false), minX(0), minY(0), maxX(GlobalVariables::chunkSize - 1), maxY(GlobalVariables::chunkSize - 1) {
		resetWorkingRect(); // Initialize working rect to an empty state
	}

	void expand(int x, int y) {
		// Mark as dirty and expand the working box
		isDirty = true;

		// Use std::min for the min coordinates and std::max for the max coordinates
		wminX = std::min(wminX, x);
		wminY = std::min(wminY, y);
		wmaxX = std::max(wmaxX, x);
		wmaxY = std::max(wmaxY, y);
	}

	void reset() {
		if (isDirty) {
			// Apply a buffer of 1 pixel and clamp to chunk boundaries (0-31)
			minX = std::clamp(wminX - 1, 0, GlobalVariables::chunkSize - 1);
			minY = std::clamp(wminY - 1, 0, GlobalVariables::chunkSize - 1);
			maxX = std::clamp(wmaxX + 1, 0, GlobalVariables::chunkSize - 1);
			maxY = std::clamp(wmaxY + 1, 0, GlobalVariables::chunkSize - 1);

			isDirty = false;
		}

		// Reset the working rectangle for the next frame
		resetWorkingRect();
	}

	int getMinX() const { return minX; }
	int getMinY() const { return minY; }
	int getMaxX() const { return maxX; }
	int getMaxY() const { return maxY; }
	bool getIsDirty() const { return isDirty; }

private:
	void resetWorkingRect() {
		// Initialize working rect to an "empty" state outside the chunk
		wminX = GlobalVariables::chunkSize;
		wminY = GlobalVariables::chunkSize;
		wmaxX = -1;
		wmaxY = -1;
	}

	bool isDirty;
	int minX, minY, maxX, maxY;
	int wminX, wminY, wmaxX, wmaxY;
};

#endif