#ifndef NAPALM_HPP
#define NAPALM_HPP
#include "BaseElements/Liquid.hpp"
#include "BaseElements/Pixel.hpp"

class Napalm : public Liquid {
public:
	Napalm();
	virtual ~Napalm();
	virtual Pixel* clone() const;


private:
	bool movingRight{ false };
};

#endif /* NAPALM_HPP */
