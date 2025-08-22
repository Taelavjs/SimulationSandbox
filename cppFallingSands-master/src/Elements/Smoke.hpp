#ifndef SMOKE_HPP
#define SMOKE_HPP

#include "BaseElements/Gas.hpp"
class Smoke : public Gas {
public:
	Smoke(const PixelType* type);
	virtual ~Smoke() = default;
	virtual Pixel* clone() const override;
};
#endif /* SMOKE_HPP */