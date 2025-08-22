#ifndef ROCK_HPP
#define ROCK_HPP

#include "BaseElements/SolidImmoveable.hpp"
#include "BaseElements/Pixel.hpp"

class Rock : public SolidImmoveable {
public:
	Rock(const PixelType* type);
	virtual ~Rock() = default;
	void update(int row, int col, WorldGeneration& worldGeneration);

	virtual Pixel* clone() const override;
};

#endif /* ROCK_HPP */