#ifndef OIL_HPP
#define OIL_HPP

#include "BaseElements/Liquid.hpp"

class Oil : public Liquid {
public:
	Oil(const PixelType* type);
	virtual ~Oil() = default;

	virtual void update(int row, int col, WorldGeneration& worldGeneration) override;
	virtual Pixel* clone() const override;
};
#endif /* OIL_HPP */