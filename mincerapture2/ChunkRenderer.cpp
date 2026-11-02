#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <array>
#include "ChunkRenderer.h"
#include "FaceVertices.h"
#include "Constants.h"
#include "BlockRegistry.h"
#include "BlockDepth.h"

ChunkRenderer::ChunkRenderer(std::shared_ptr<ChunkData> data) : chunkData(data) 
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
}

ChunkRenderer::~ChunkRenderer()
{
	glDeleteVertexArrays(1, &VAO);
}

void ChunkRenderer::Render()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ChunkRenderer::GenerateChunkMesh()
{
	vertices.clear();
	indices.clear();

	glm::vec3 faceDirs[]{ FrontNormal, BackNormal, LeftNormal, RightNormal, TopNormal, BottomNormal };
	for (int layer = 0; layer < 2; layer++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				glm::ivec2 local{ x, y };
				short id = chunkData->GetBlock(local, layer);
				if (id == 0) continue;
				auto& bd = BlockRegistry::GetBlock(id);
				
				for (int face = 0; face < 6; face++)
				{
					//face class with list of them later
					auto checkedblock = glm::ivec2{ x + faceDirs[face].x, y + faceDirs[face].y };
					int checked;
					switch (face)
					{
					case Faces::BACK:
						break;
					case Faces::FRONT:
						if (layer)
						{
							AddFace(x, y, bd.depth, bd, face);
						}
						else
						{
							if (chunkData->GetBlock(checkedblock, 1)) break;
							AddFace(x, y, 0, bd, face);
						}
						break;
					case Faces::LEFT:
						checked = checkedblock.x < 0 ? 0 : chunkData->GetBlock(checkedblock, layer);
						if (layer)
						{
							if (checked)
							{
								auto& cbd = BlockRegistry::GetBlock(checked);
								if (cbd.depth >= bd.depth) break;
							}
							for (int d = 0; d < bd.depth; d++)
							{
								AddFace(x, y, d + layer, bd, face);
							}
						}
						else
						{
							if (checked) break;
							AddFace(x, y, 0, bd, face);
						}
						break;
					case Faces::RIGHT:
						checked = checkedblock.x >= CHUNK_WIDTH ? 0 : chunkData->GetBlock(checkedblock, layer);
						if (layer)
						{
							if (checked)
							{
								auto& cbd = BlockRegistry::GetBlock(checked);
								if (cbd.depth >= bd.depth) break;
							}
							for (int d = 0; d < bd.depth; d++)
							{
								AddFace(x, y, d + layer, bd, face);
							}
						}
						else
						{
							if (checked) break;
							AddFace(x, y, 0, bd, face);
						}
						break;
					case Faces::TOP:
						checked = checkedblock.y >= CHUNK_HEIGHT ? 0 : chunkData->GetBlock(checkedblock, layer);
						if (layer)
						{
							if (checked)
							{
								auto& cbd = BlockRegistry::GetBlock(checked);
								if (cbd.depth >= bd.depth) break;
							}
							for (int d = 0; d < bd.depth; d++)
							{
								AddFace(x, y, d + layer, bd, face);
							}
						}
						else
						{
							if (checked) break;
							AddFace(x, y, 0, bd, face);
						}
						break;
					case Faces::BOTTOM:
						checked = checkedblock.y < 0 ? 0 : chunkData->GetBlock(checkedblock, layer);
						if (layer)
						{
							if (checked)
							{
								auto& cbd = BlockRegistry::GetBlock(checked);
								if (cbd.depth >= bd.depth) break;
							}
							for (int d = 0; d < bd.depth; d++)
							{
								AddFace(x, y, d + layer, bd, face);
							}
						}
						else
						{
							if (checked) break;
							AddFace(x, y, 0, bd, face);
						}
						break;
					}
				}
			}
		}
	}
}
//seperate vertices for blocks with both face sides rendered (like leaves or fire, basically anything with alpha)
void ChunkRenderer::AddFace(int x, int y, int z, BlockData& bd, int face)
{
	auto& fd = FaceDatas[face];

	std::array<glm::vec2, 4> uvs = bd.GetUVs(face);
	glm::vec3 position = glm::vec3(x, y, z) + glm::vec3(chunkData->ChunkPos.x * CHUNK_WIDTH, chunkData->ChunkPos.y * CHUNK_HEIGHT, 0.0f);
	for (int i = 0; i < fd.Vertices.size(); i++)
	{
		vertices.push_back(Vertex(fd.Vertices[i] + position, fd.Normal, uvs[i]));
	}
	for (auto indice : FaceIndices)
	{
		indices.push_back(vertices.size() - 4 + indice);
	}
}