#ifndef SOLIDIMMOVEABLE_HPP
#define SOLIDIMMOVEABLE_HPP

#include "Pixel.hpp"

class SolidImmoveable : public Pixel {
public:
	SolidImmoveable(const PixelType* type);
	virtual ~SolidImmoveable() = default;
	virtual void update(int row, int col, WorldGeneration& worldGeneration) override;
	virtual Pixel* clone() const override;
};

#endif /* SOLIDIMMOVEABLE_HPP */