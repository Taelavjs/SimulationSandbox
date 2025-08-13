#ifndef SUBCHUNKBOUNDINGBOX
#define SUBCHUNKBOUNDINGBOX

class SubChunkBoundingBox
{
public:
	SubChunkBoundingBox(int rowStart, int rowEnd, int colStart, int colEnd);
	void updateSubChunk();
	void updateNextBoundingBox(
		int rS, int rE, int cS, int cE
	);

	int subChunkRowStart;
	int subChunkColStart;
	int subChunkRowEnd;
	int subChunkColEnd;

	int nextSubRowStart;
	int nextSubColStart;
	int nextSubRowEnd;
	int nextSubColEnd;


};

#endif
