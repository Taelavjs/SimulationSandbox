#include "SolidImmoveable.hpp"
#include "Pixel.hpp"

SolidImmoveable::SolidImmoveable() {
	setIsMoveable(false);
	setIsSolid(true);
}
SolidImmoveable::~SolidImmoveable() {

}
