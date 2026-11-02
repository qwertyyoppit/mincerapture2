#pragma once
#include <glm/glm.hpp>
#include <array>

class Sprite
{
public:
	std::array<glm::vec2, 4> uv;
	Sprite(std::array<float, 4> rect = { { 0.0f, 0.0f, 1.0f, 1.0f } });

};