#ifndef Oil_HPP
#define Oil_HPP
#include "BaseElements/Liquid.hpp"

class Oil : public Liquid {
public:
	Oil();
	virtual ~Oil();
	virtual Pixel* clone() const;

public:
	uint32_t getColour() override;

};

#endif /* Oil_HPP */
