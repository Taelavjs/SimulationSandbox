#ifndef WATER_hpp
#define WATER_hpp
#include "BaseElements/Liquid.hpp"
class Pixel;
class Water : public Liquid {
public:
	Water();
	virtual ~Water();
	virtual Pixel* clone() const;
	int getDensity() const;

private:
	bool movingRight{ false };
};

#endif /* WATER_hpp */
