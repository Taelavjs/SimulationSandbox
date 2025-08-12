#ifndef ROCK_HPP
#define ROCK_HPP
#include "BaseElements/SolidImmoveable.hpp"
#include "BaseElements/Pixel.hpp"
#include <iostream>

class Rock : public SolidImmoveable {
public:
    Rock();
    virtual ~Rock();
    virtual Pixel *clone() const { return new Rock(); }

};

#endif /* ROCK_HPP */
