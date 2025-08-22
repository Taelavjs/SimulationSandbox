#ifndef WATER_HPP
#define WATER_HPP

#include "BaseElements/Liquid.hpp"

class Water : public Liquid {
public:
	Water(const PixelType* type);
	virtual ~Water() = default;

	virtual void update(int row, int col, WorldGeneration& worldGeneration) override;

	virtual Pixel* clone() const override;
};
#endif /* WATER_HPP */