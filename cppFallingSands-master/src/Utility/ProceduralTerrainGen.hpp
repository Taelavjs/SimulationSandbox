#ifndef PROCEDURALTERRAINGEN
#define PROCEDURALTERRAINGEN
#include <iostream>
#include <vector>
#include "FastNoiseLite.h"
class ProceduralTerrainGen {
private:
	ProceduralTerrainGen();
public:
	~ProceduralTerrainGen();

	static std::vector<float> createNoise(int w, int h);
	static std::vector<float> createTerrain(int w, int h);

	//void iterThroughPixels(void (*func)(int &, int &, uint32_t *, uint32_t *), uint32_t *pixels, uint32_t *newPixels);

};

#endif /* PROCEDURALTERRAINGEN */
