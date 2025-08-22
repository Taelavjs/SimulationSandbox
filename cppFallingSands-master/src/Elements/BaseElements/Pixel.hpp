#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "SDL.h"
#include "../../GameSetup/World/Chunk.hpp"
#include "../../GameSetup/World/WorldGeneration.hpp"

class WorldGeneration;

// Flyweight: Stores static, shared properties for each pixel type
class PixelType {
public:
	// Core properties
	const bool isMoveable;
	const bool isLiquid;
	const bool isSolid;
	const bool isGas;
	const bool isFlammable;
	const int mass;
	const int density;
	const double chanceToIgnite;
	const uint32_t color;

	// Constructor to initialize all const properties
	PixelType(
		bool moveable, bool liquid, bool solid, bool gas, bool flammable,
		int mass, int density, double chanceToIgnite, uint32_t color)
		: isMoveable(moveable), isLiquid(liquid), isSolid(solid), isGas(gas),
		isFlammable(flammable), mass(mass), density(density),
		chanceToIgnite(chanceToIgnite), color(color) {
	}
};

// Default Base Class of every Pixel
class Pixel {
public:
	// This is the only link to the shared data
	const PixelType* type;

	// Dynamic state properties (unique to each pixel)
private:
	bool onFire{ false };
	bool isProcessed{ false };
	int hp{ 2 };

public:
	// Getters for dynamic state
	bool getOnFire() const { return onFire; }
	int getHp() const { return hp; }
	bool getProcessed() const { return isProcessed; }

	// Setters for dynamic state
	void setProcessed(bool val) { isProcessed = val; }
	void setOnFire() { onFire = true; }
	void setHp(int health) { hp = health; }

public:
	Pixel(const PixelType* type);
	virtual ~Pixel() = default;
	virtual Pixel* clone() const = 0;
	virtual void update(int row, int col, WorldGeneration& worldGeneration) {};

	// FIRE FUNCTIONALITY
	virtual void ignite();
	virtual bool hit();
	virtual bool fireTick(WorldGeneration& worldGeneration, const int& row, const int& col);
};

#endif // PIXEL_HPP