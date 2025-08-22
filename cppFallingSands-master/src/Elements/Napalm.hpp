#ifndef NAPALM_HPP
#define NAPALM_HPP

#include "BaseElements/Liquid.hpp"

class Napalm : public Liquid {
public:
	Napalm(const PixelType* type);
	virtual ~Napalm() = default;
	virtual Pixel* clone() const override;

private:
	bool movingRight{ false };
};
#endif /* NAPALM_HPP */