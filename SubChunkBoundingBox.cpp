#include "SubChunkBoundingBox.hpp"

SubChunkBoundingBox::SubChunkBoundingBox(int rowStart, int rowEnd, int colStart, int colEnd)
	:subChunkRowStart(rowStart), subChunkColStart(colStart), subChunkRowEnd(rowEnd), subChunkColEnd(colEnd),
	nextSubRowStart(-1), nextSubRowEnd(-1), nextSubColStart(-1), nextSubColEnd(-1)
{

}

void SubChunkBoundingBox::updateSubChunk() {
	subChunkRowStart = nextSubRowStart;
	subChunkRowEnd = nextSubRowEnd;
	subChunkColStart = nextSubColStart;
	subChunkColEnd = nextSubColEnd;

	nextSubRowStart = -1;
	nextSubRowEnd = -1;
	nextSubColStart = -1;
	nextSubColEnd = -1;
}

void SubChunkBoundingBox::updateNextBoundingBox(
	int rS, int rE, int cS, int cE
) {
	if (subChunkRowStart == -1) {
		nextSubRowStart = rS;
		nextSubRowEnd = rE;
		nextSubColStart = cS;
		nextSubColEnd = cE;
		return;
	}

	if (rS < subChunkRowStart) nextSubRowStart = rS;
	if (rE > subChunkRowStart) nextSubRowEnd = rE;
	if (cS < subChunkRowStart) nextSubColStart = cS;
	if (cE > subChunkRowStart) nextSubColEnd = cE;

	return;
}
