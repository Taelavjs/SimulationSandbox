#ifndef WATER_hpp
#define WATER_hpp
#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"
#include "BaseElements/Liquid.hpp"

class Water : public Liquid {
public:
    Water();
    virtual ~Water();
    virtual Pixel *clone() const { return new Water(); }
    int getDensity(){return 5;};

private:
    bool movingRight{false};
};

#endif /* WATER_hpp */
