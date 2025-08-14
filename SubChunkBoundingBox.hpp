#ifndef SUBCHUNKBOUNDINGBOX
#define SUBCHUNKBOUNDINGBOX

class chunkBoundingBox {
public:
	chunkBoundingBox();
	void expand(int x, int y);

	void reset();

	int getMinX() const;
	int getMinY() const;
	int getMaxX() const;
	int getMaxY() const;
	bool getIsDirty() const;

private:
	void resetWorkingRect();
	bool isDirty;
	int minX, minY, maxX, maxY;
	int wminX, wminY, wmaxX, wmaxY;
};

#endif