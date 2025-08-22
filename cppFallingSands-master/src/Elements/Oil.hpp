#ifndef OIL_HPP
#define OIL_HPP

#include "BaseElements/Liquid.hpp"

class Oil : public Liquid {
public:
	Oil(const PixelType* type);
	virtual ~Oil() = default;

	virtual Pixel* clone() const override;
};
#endif /* OIL_HPP */