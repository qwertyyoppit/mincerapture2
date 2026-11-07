#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <shared_mutex>
#include <string>
#include "ChunkConstants.h"
#include "ChunkData.h"
#include "ChunkRenderer.h"
#include "texture_from_file.h"
#include "Shader.h"
#include "Camera.h"
#include "FastNoiseLite.h"

class WorldGen
{
public:
	WorldGen();

	int GetBlock(glm::ivec3 block);

	int GetBlock(int x, int y, int z);

	void SetBlock(glm::ivec3 block, int id);

	void SetBlock(int x, int y, int z, int id);

	std::shared_ptr<ChunkData> GetChunkData(glm::ivec3 chunk);

	std::shared_ptr<ChunkData> GetOrGenerateChunkData(glm::ivec3 chunk);

	inline glm::ivec3 WorldToBlock(glm::vec3 pos)
	{
		return WorldToBlock(pos.x, pos.y, pos.z);
	}

	inline glm::ivec3 WorldToBlock(float x, float y, float z)
	{
		return glm::ivec3(round(x), round(y), round(z));
	}

	inline glm::ivec3 WorldToChunk(glm::vec3 pos)
	{
		return BlockToChunk(WorldToBlock(pos));
	}

	inline glm::ivec3 BlockToChunk(glm::ivec3 block)
	{
		return BlockToChunk(block.x, block.y, block.z);
	}

	inline glm::ivec3 BlockToChunk(int x, int y, int z)
	{
		int chunkX = x < 0 ? floor(x / (float)CHUNK_SIZE) : x / CHUNK_SIZE;
		int chunkY = y < 0 ? floor(y / (float)CHUNK_SIZE) : y / CHUNK_SIZE;
		int chunkZ = z < 0 ? floor(z / (float)CHUNK_SIZE) : z / CHUNK_SIZE;

		return glm::ivec3(chunkX, chunkY, chunkZ);
	}

	inline glm::ivec3 WorldToChunkPos(glm::vec3 pos, glm::ivec3 chunk)
	{
		return BlockToChunkPos(WorldToBlock(pos), chunk);
	}

	inline glm::ivec3 BlockToChunkPos(glm::ivec3 block, glm::ivec3 chunk)
	{
		return BlockToChunkPos(block.x, block.y, block.z, chunk);
	}

	inline glm::ivec3 BlockToChunkPos(int x, int y, int z, glm::ivec3 chunk)
	{
		return glm::ivec3(x, y, z) - chunk * CHUNK_SIZE;
	}

	inline glm::ivec3 ChunkPosToBlock(glm::ivec3 chunkPos, glm::ivec3 chunk)
	{
		return ChunkPosToBlock(chunkPos.x, chunkPos.y, chunkPos.z, chunk);
	}

	inline glm::ivec3 ChunkPosToBlock(int x, int y, int z, glm::ivec3 chunkPos)
	{
		return glm::ivec3(x, y, z) + chunkPos * CHUNK_SIZE;
	}

	void Render();

	unsigned int grass;

	Shader chunkShader = Shader("cube_shader.vs", "cube_shader.fsh");

	Camera* camera = nullptr;

//private:

	std::unordered_map<glm::ivec3, std::shared_ptr<ChunkData>> WorldData;

	std::unordered_map <glm::ivec3, std::shared_ptr<ChunkRenderer>> ChunkRenderers;

	void generateChunk(ChunkData* data);
};"

WorldGen.cpp:
"#include "WorldGen.h"

WorldGen::WorldGen()
{
	grass = TextureFromFile("atlas.png", ".");
}

void WorldGen::generateChunk(ChunkData* data)
{
	FastNoiseLite noise;
	noise.SetNoiseType(noise.NoiseType_Perlin);

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				auto block = ChunkPosToBlock(glm::ivec3(x, y, z), data->chunkPos);
				float noisePosX = (data->chunkPos.x + (float)x / (float)CHUNK_SIZE) * CHUNK_SIZE;
				float noisePosY = (data->chunkPos.z + (float)z / (float)CHUNK_SIZE) * CHUNK_SIZE;
				float noiseScale = 5.0f;
				int heightOffset = 15 * (noise.GetNoise(noisePosX * noiseScale, noisePosY * noiseScale) + 1) / 2;
				int height = CHUNK_SIZE + heightOffset;
				if (block.y == height - 1) data->Set(x, y, z, 2);
				else if (block.y < height - 5) data->Set(x, y, z, 3);
				else if (block.y < height - 1) data->Set(x, y, z, 1);
			}
		}
	}
}

int WorldGen::GetBlock(glm::ivec3 block)
{
	return GetBlock(block.x, block.y, block.z);
}

int WorldGen::GetBlock(int x, int y, int z)
{
	auto chunk = BlockToChunk(x, y, z);
	auto chunkData = GetChunkData(chunk);
	
	if (!chunkData) return 0;

	auto chunkPos = BlockToChunkPos(x, y, z, chunk);
	return chunkData->Get(chunkPos.x, chunkPos.y, chunkPos.z);
}

void WorldGen::SetBlock(glm::ivec3 block, int id)
{
	SetBlock(block.x, block.y, block.z, id);
}

void WorldGen::SetBlock(int x, int y, int z, int id)
{
	auto chunk = BlockToChunk(x, y, z);
	auto chunkData = GetChunkData(chunk);

	if (!chunkData) return;

	auto chunkPos = BlockToChunkPos(x, y, z, chunk);
	chunkData->Set(chunkPos.x, chunkPos.y, chunkPos.z, id);
}

std::shared_ptr<ChunkData> WorldGen::GetChunkData(glm::ivec3 chunk)
{
	if (WorldData.find(chunk) == WorldData.end()) return nullptr;

	return WorldData[chunk];
}

std::shared_ptr<ChunkData> WorldGen::GetOrGenerateChunkData(glm::ivec3 chunk)
{
	auto chunkData = GetChunkData(chunk);
	if (chunkData)
		return chunkData;

	chunkData = std::make_shared<ChunkData>(chunk);
	WorldData[chunk] = chunkData;
	generateChunk(chunkData.get());

	return chunkData;
}

void WorldGen::Render()
{
	// No deletion of chunks yet
	auto camChunk = WorldToChunk(camera->Position);

	std::vector<std::shared_ptr<ChunkData>> renderedChunkData;
	for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE + 1; x++)
	{
		for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE + 1; y++)
		{
			for (int z = -RENDER_DISTANCE; z < RENDER_DISTANCE + 1; z++)
			{
				glm::ivec3 chunk = glm::ivec3(x, y, z) + camChunk;
				auto chunkData = GetChunkData(chunk);
				if (!chunkData)
				{
					chunkData = std::make_shared<ChunkData>(chunk);
					WorldData[chunk] = chunkData;
					generateChunk(chunkData.get());
				}
				renderedChunkData.push_back(chunkData);
			}
		}
	}
	for (auto& data : renderedChunkData)
	{
		if (ChunkRenderers.find(data->chunkPos) == ChunkRenderers.end())
		{
			auto renderer = std::make_shared<ChunkRenderer>(data);
			renderer->topChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(0, 1, 0));
			renderer->bottomChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(0, -1, 0));
			renderer->leftChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(-1, 0, 0));
			renderer->rightChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(1, 0, 0));
			renderer->frontChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(0, 0, 1));
			renderer->backChunkData = GetOrGenerateChunkData(data->chunkPos + glm::ivec3(0, 0, -1));
			ChunkRenderers[data->chunkPos] = renderer;
			renderer->generateMesh();
		}
	}
	for (auto& renderer : ChunkRenderers)
	{
		renderer.second->Render();
	}
}
