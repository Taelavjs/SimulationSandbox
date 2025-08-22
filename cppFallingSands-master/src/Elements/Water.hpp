#ifndef WATER_HPP
#define WATER_HPP

#include "BaseElements/Liquid.hpp"

class Water : public Liquid {
public:
	Water(const PixelType* type);
	virtual ~Water() = default;
	virtual Pixel* clone() const override;
};
#endif /* WATER_HPP */