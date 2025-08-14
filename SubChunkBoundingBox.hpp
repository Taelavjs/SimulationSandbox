#ifndef SUBCHUNKBOUNDINGBOX
#define SUBCHUNKBOUNDINGBOX

class chunkBoundingBox {
public:
	chunkBoundingBox();
	void expand(int x, int y);

	void reset();

	const int& getMinX() const;
	const int& getMinY() const;
	const int& getMaxX() const;
	const int& getMaxY() const;
	const bool& getIsDirty() const;

private:
	void resetWorkingRect();
	bool isDirty;
	int minX, minY, maxX, maxY;
	int wminX, wminY, wmaxX, wmaxY;
};

#endif