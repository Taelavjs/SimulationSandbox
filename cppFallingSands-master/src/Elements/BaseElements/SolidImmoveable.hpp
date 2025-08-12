#ifndef SOLIDIMMOVEABLE_HPP
#define SOLIDIMMOVEABLE_HPP
#include "Pixel.hpp"

class SolidImmoveable : public Pixel {
public:
    SolidImmoveable(){setIsMoveable(false); setIsSolid(true);};
    ~SolidImmoveable(){};
};

#endif /* SOLIDIMMOVEABLE_HPP */
