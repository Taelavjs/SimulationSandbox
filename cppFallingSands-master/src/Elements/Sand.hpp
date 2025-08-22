#ifndef SAND_HPP
#define SAND_HPP

#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"
class Sand : public SolidDynamic {
public:
	Sand(const PixelType* type);
	virtual ~Sand() = default;
	virtual Pixel* clone() const override;
};

#endif /* SAND_HPP */