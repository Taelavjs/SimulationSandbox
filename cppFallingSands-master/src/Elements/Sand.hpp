#ifndef Sand_hpp
#define Sand_hpp
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include "BaseElements/SolidDynamic.hpp"
#include "BaseElements/Pixel.hpp"

class Sand : public SolidDynamic {
public:
    Sand();
    virtual ~Sand();
    virtual Pixel *clone() const { return new Sand(); }

};


#endif /* Sand_hpp */
