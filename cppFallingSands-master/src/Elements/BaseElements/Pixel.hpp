#ifndef PIXEL_HPP
#define PIXEL_HPP

#include "SDL.h"
#include "../../GameSetup/World/Chunk.hpp"
#include "../../GameSetup/World/WorldGeneration.hpp"

class WorldGeneration;
// Default Base Class Of Every Pixel
class Pixel {

	// GETTERS ELEMENTAL FEATURES
public:
	bool getIsMoveable();
	bool getIsLiquid();
	bool getIsSolid();
	bool getIsGas();
	bool getIsFlammable();

	// SETTERS ELEMENTAL FEATURES
	void setIsMoveable(bool val);
	void setIsLiquid(bool val);
	void setIsSolid(bool val);
	void setIsGas(bool val);
	void setIsFlammable(bool val);

	// GETTERS FUNCTIONS
public:
	bool getProcessed();
	int getMass();
	bool getOnFire();
	virtual uint32_t getColour();
	double getChanceToIgnite();
	int getHp();
	int getDensity();
	int getTerminalX();
	int getTerminalY();
	// SETTERS FUNCTIONS
public:
	void setProcessed(bool val);
	void setFlammable(bool val);
	void setOnFire();
	void setHp(int health);
	void setMass(int val);
	void setChanceToIgnite(double val);
	void setDensity(int val);
	virtual void setColor(uint32_t val);

public:
	//Elemental Bools
	bool isMoveable{ false };
	bool isFlammable{ false };
	bool isGas{ false };
	bool isLiquid{ false };
	bool isSolid{ false };
private:
	// Others
	int hp{ 5 };
	bool onFire{ false };
	uint32_t color{ SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32), 255, 255, 255, 255) };
	double chanceToIgnite{ 0.9f };
	bool isProcessed{ false };
	int mass{ 1 };
	int density{ 0 };
	int terminalX{ 2 };
	int terminalY{ 2 };

public:
	Pixel();
	virtual ~Pixel();
	virtual Pixel* clone() const = 0;
	virtual void update(int row, int col, const int& vecWidth, const int& vecHeight, WorldGeneration& worldGeneration) {};

	// FIRE FUNCTIONALITY
public:
	virtual void ignite();
	virtual bool hit();
	virtual bool fireTick(WorldGeneration& worldGeneration, const int& row, const int& col);
};
#endif // PIXEL_HPP
