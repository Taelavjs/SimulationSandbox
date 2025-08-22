#ifndef MOVEABLE_HPP
#define MOVEABLE_HPP

#include "Pixel.hpp"
#include "../../GameSetup/World/Chunk.hpp"

class Moveable : public Pixel {
public:
	Moveable(const PixelType* type);
	virtual ~Moveable() = default;
	int x_direction{ 0 };
protected:
	float randomNumber();
};

#endif /* MOVEABLE_HPP */