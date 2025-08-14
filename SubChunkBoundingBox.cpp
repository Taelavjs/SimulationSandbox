#include "SubChunkBoundingBox.hpp"
#include <algorithm> // For std::min and std::max
#include "../FallingSandsSumoSimulation/cppFallingSands-master/src/Utility/GlobalVariables.hpp"

chunkBoundingBox::chunkBoundingBox() : isDirty(false), minX(0), minY(0), maxX(GlobalVariables::chunkSize - 1), maxY(GlobalVariables::chunkSize - 1) {
	resetWorkingRect();
}

void chunkBoundingBox::expand(int x, int y) {
	isDirty = true;

	wminX = std::min(wminX, x);
	wminY = std::min(wminY, y);
	wmaxX = std::max(wmaxX, x);
	wmaxY = std::max(wmaxY, y);
}

void chunkBoundingBox::reset() {
	if (isDirty) {
		minX = std::clamp(wminX - 1, 0, GlobalVariables::chunkSize - 1);
		minY = std::clamp(wminY - 1, 0, GlobalVariables::chunkSize - 1);
		maxX = std::clamp(wmaxX + 1, 0, GlobalVariables::chunkSize - 1);
		maxY = std::clamp(wmaxY + 1, 0, GlobalVariables::chunkSize - 1);

		isDirty = false;
	}

	resetWorkingRect();
}

const int& chunkBoundingBox::getMinX() const { return minX; }
const int& chunkBoundingBox::getMinY() const { return minY; }
const int& chunkBoundingBox::getMaxX() const { return maxX; }
const int& chunkBoundingBox::getMaxY() const { return maxY; }
const bool& chunkBoundingBox::getIsDirty() const { return isDirty; }

void chunkBoundingBox::resetWorkingRect() {
	wminX = GlobalVariables::chunkSize;
	wminY = GlobalVariables::chunkSize;
	wmaxX = -1;
	wmaxY = -1;
}
