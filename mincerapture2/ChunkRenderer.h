#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <vector>
#include <shared_mutex>
#include "ChunkData.h"
#include "BlockData.h"
#include "Faces.h"

class ChunkRenderer
{
public:

	struct Vertex
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 UVs;
		Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uvs) : Position(position), Normal(normal), UVs(uvs) {}
	};

	ChunkRenderer(std::shared_ptr<ChunkData> data);
	~ChunkRenderer();

	void Render();

	void GenerateChunkMesh();

	void AddFace(int x, int y, int z, BlockData& bd, int face);

private:
	std::shared_ptr<ChunkData> chunkData;

	//separate mesh data and new mesh data until fully generated so current mesh data can always be rendered
	std::vector<Vertex> vertices{};
	std::vector<int> indices{};

	unsigned int VBO, EBO, VAO;
};