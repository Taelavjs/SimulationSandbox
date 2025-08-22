#include "Napalm.hpp"
#include <random>

Napalm::Napalm(const PixelType* type) : Liquid(type) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);
	movingRight = (dis(gen) > 0.5f);

	setOnFire();
}

Pixel* Napalm::clone() const {
	return new Napalm(type);
}

void Napalm::update(int row, int col, WorldGeneration& worldGeneration) {
	Liquid::update(row, col, worldGeneration);
}