#pragma once

#include <glm/glm.hpp>

class Entity
{
public:
	//
	glm::vec2 floatPos{};
	//unsigned long pos_x{};
	//unsigned int pos_y{};
	long pos_x{};
	int pos_y{};

	virtual void Update()=0;

	virtual void Render()=0;
};