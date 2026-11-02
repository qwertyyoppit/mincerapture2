#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "Faces.h"
#include "BlockDepth.h"

class BlockData
{
public:
	std::string name;

	BlockDepth::BlockDepth layer = BlockDepth::Fore;

	int depth = 2;

	int hardness = 1;
	//window overrides back but IS TRANSPARENT to middle layer
	bool overridesback{};
	
	BlockData(std::array<glm::vec2, 4> uv);
	BlockData(std::array<glm::vec2, 4> sideuv, std::array<glm::vec2, 4> topuv);
	BlockData(std::array<glm::vec2, 4> sideuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv);
	BlockData(std::array<glm::vec2, 4> frontbackuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv, std::array<glm::vec2, 4> leftrightuv);
	BlockData(std::array<glm::vec2, 4> frontbackuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv, std::array<glm::vec2, 4> leftuv, std::array<glm::vec2, 4> rightuv);
	BlockData();

	std::array<glm::vec2, 4> GetUVs(int face) const
	{
		switch (face)
		{
		case Faces::FRONT:
			return FrontUVs;
		case Faces::BACK:
			return BackUVs;
		case Faces::LEFT:
			return LeftUVs;
		case Faces::RIGHT:
			return RightUVs;
		case Faces::TOP:
			return TopUVs;
		case Faces::BOTTOM:
			return BottomUVs;
		}
	}

	std::function<void(short int)> OnInteract;

private:
	std::array<glm::vec2, 4> FrontUVs;
	std::array<glm::vec2, 4> BackUVs;
	std::array<glm::vec2, 4> LeftUVs;
	std::array<glm::vec2, 4> RightUVs;
	std::array<glm::vec2, 4> TopUVs;
	std::array<glm::vec2, 4> BottomUVs;
};