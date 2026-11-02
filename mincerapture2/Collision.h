#pragma once
#include <array>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/ext.hpp>
namespace Collision
{
	const static bool BoxCollide(const std::array<glm::vec2, 4>& box1, const std::array<glm::vec2, 4>& box2)
	{
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 side[2] = { box1[i], box1[(i + 1) % 4] };
			glm::vec2 rels = glm::normalize(side[1] - side[0]);
			float theta = asinf(rels.y);
			side[1] = glm::vec2((side[1].x - side[0].x) / cosf(theta) + side[0].x, side[0].y);
			for (int j = 0; j < 4; j++)
			{
				glm::vec2 side2[2] = { box2[j], box2[(j + 1) % 4] };
				glm::vec2 rels2[2] = {glm::normalize(side2[0] - side[0]), glm::normalize(side2[1] - side[0]) };
				side2[0] = glm::vec2((side2[0].x - side[0].x) / cosf(acosf(rels2[0].x) - theta), (side2[0].y - side[0].y) / sinf(asinf(rels2[0].y) - theta)) + side[0];
				side2[1] = glm::vec2((side2[1].x - side[0].x) / cosf(acosf(rels2[1].x) - theta), (side2[1].y - side[0].y) / sinf(asinf(rels2[1].y) - theta)) + side[0];
				float& x = side[0].x, y = side[0].y;
				// Check if box 1 side y is between the box 2 side points
				if (!(y > fmin(side2[0].y, side2[1].y) && y < fmax(side2[0].y, side2[1].y))) continue;
				float xon = side2[0].x + (side2[1].x - side[0].x) * (y - side2[0].y) / (side2[1].y - side2[0].y);
				if (xon > side[0].x && x < side[1].x) return true;
			}
		}

		return false;
	}
};