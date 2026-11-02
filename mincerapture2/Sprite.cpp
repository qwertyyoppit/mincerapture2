#include "Sprite.h"

Sprite::Sprite(std::array<float, 4> rect)
{
	float EPSILON = 1e-6;
	rect[0] += EPSILON;
	rect[1] += EPSILON;
	rect[2] += EPSILON;
	rect[3] -= EPSILON;
	uv = { { { rect[0], rect[1] }, { rect[2], rect[1] }, { rect[0], rect[3] }, { rect[2], rect[3] } } };
}