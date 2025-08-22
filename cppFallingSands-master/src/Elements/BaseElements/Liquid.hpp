#ifndef LIQUID_HPP
#define LIQUID_HPP

#include "Moveable.hpp"
class Liquid : public Moveable {
public:
	Liquid(const PixelType* type);
	virtual ~Liquid() = default;

	virtual void update(int row, int col, WorldGeneration& worldGeneration) override;

	virtual Pixel* clone() const override;
};

#endif /* LIQUID_HPP */