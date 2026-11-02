#pragma once
#include <glm/glm.hpp>
#include <vector>

const std::vector<int> FaceIndices
{
	0, 2, 1,
	2, 3, 1
};

const std::vector<glm::vec3> FrontVerts
{
	{ 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f }
};

const std::vector<glm::vec3> BackVerts
{
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }
};

const std::vector<glm::vec3> LeftVerts
{
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};

const std::vector<glm::vec3> RightVerts
{
	{ 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 0.0f }
};

const std::vector<glm::vec3> TopVerts
{
	{ 0.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f }
};

const std::vector<glm::vec3> BottomVerts
{
	{ 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f }
};

const glm::vec3 FrontNormal{ 0.0f, 0.0f, 1.0f };
const glm::vec3 BackNormal{ 0.0f, 0.0f, -1.0f };
const glm::vec3 LeftNormal{ -1.0f, 0.0f, 0.0f };
const glm::vec3 RightNormal{ 1.0f, 0.0f, 0.0f };
const glm::vec3 TopNormal{ 0.0f, 1.0f, 0.0f };
const glm::vec3 BottomNormal{ 0.0f, -1.0f, 0.0f };

class FaceData
{
public:
	std::vector<glm::vec3> Vertices;
	glm::vec3 Normal;
	//glm::vec2 UV;

	FaceData(std::vector<glm::vec3> vertices, glm::vec3 normal) : Vertices(vertices), Normal(normal) {}
};

const FaceData FaceDatas[]{ {FrontVerts, FrontNormal }, { BackVerts, BackNormal }, { LeftVerts, LeftNormal }, {RightVerts, RightNormal}, { TopVerts, TopNormal }, { BottomVerts, BottomNormal } };