// --- Gas.hpp ---
#ifndef GAS_HPP
#define GAS_HPP

#include "Moveable.hpp"

class Gas : public Moveable {
public:
	Gas(const PixelType* type);
	virtual ~Gas() = default;
	void update(int row, int col, WorldGeneration& worldGeneration) override;

private:
	int x_direction{ 0 };
	int yVelocity{ 1 };
};

#endif /* GAS_HPP */