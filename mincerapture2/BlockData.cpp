#include "BlockData.h"

BlockData::BlockData(std::array<glm::vec2, 4> uv)
	: FrontUVs(uv), BackUVs(uv), LeftUVs(uv), RightUVs(uv), TopUVs(uv), BottomUVs(uv)
{ }
BlockData::BlockData(std::array<glm::vec2, 4> sideuv, std::array<glm::vec2, 4> topuv)
	: FrontUVs(sideuv), BackUVs(sideuv), LeftUVs(sideuv), RightUVs(sideuv), TopUVs(topuv), BottomUVs(topuv)
{ }
BlockData::BlockData(std::array<glm::vec2, 4> sideuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv)
	: FrontUVs(sideuv), BackUVs(sideuv), LeftUVs(sideuv), RightUVs(sideuv), TopUVs(topuv), BottomUVs(bottomuv)
{ }
BlockData::BlockData(std::array<glm::vec2, 4> frontbackuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv, std::array<glm::vec2, 4> leftrightuv)
	: FrontUVs(frontbackuv), BackUVs(frontbackuv), LeftUVs(leftrightuv), RightUVs(leftrightuv), TopUVs(topuv), BottomUVs(topuv)
{ }
BlockData::BlockData(std::array<glm::vec2, 4> frontbackuv, std::array<glm::vec2, 4> topuv, std::array<glm::vec2, 4> bottomuv, std::array<glm::vec2, 4> leftuv, std::array<glm::vec2, 4> rightuv)
	: FrontUVs(frontbackuv), BackUVs(frontbackuv), LeftUVs(leftuv), RightUVs(rightuv), TopUVs(topuv), BottomUVs(topuv)
{ }

BlockData::BlockData() {}