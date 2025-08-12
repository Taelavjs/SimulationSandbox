#ifndef SMOKE_HPP
#define SMOKE_HPP
#include "BaseElements/Gas.hpp"
#include "BaseElements/Pixel.hpp"

class Smoke;
class Smoke : public Gas {
public:
    Smoke();
    virtual ~Smoke();
    virtual Pixel *clone() const { return new Smoke(); }
};
#endif /* SMOKE_HPP*/
