#include "Moveable.hpp"
#include <chrono>
#include <random>

Moveable::Moveable(const PixelType* type) : Pixel(type) {}

float Moveable::randomNumber() {
	static std::default_random_engine rng(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
	static std::uniform_real_distribution<double> dist(0.0, 1.0);
	return dist(rng);
}