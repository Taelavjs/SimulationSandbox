#ifndef PROCEDURALTERRAINGEN
#define PROCEDURALTERRAINGEN
#include "FastNoiseLite.h"
#include <vector>

class ProceduralTerrainGen {
public:
	static std::vector<float> createNoise(int w, int h);
	static std::vector<float> createTerrain(int w, int h);

private:
	static std::vector<float> gatherNoiseData(FastNoiseLite& noise, int w, int h);
};

#endif
