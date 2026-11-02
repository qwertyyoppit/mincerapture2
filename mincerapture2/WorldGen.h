#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <shared_mutex>
#include "Constants.h"
#include "ChunkData.h"
#include "ChunkRenderer.h"
#include "Texture.h"
#include "LoadTextureFromFile.h"

class WorldGen
{
public:
	WorldGen();

	void GenerateChunkData(glm::ivec2 chunk);

	static glm::ivec2 WorldToBlock(glm::vec2 pos)
	{
		return WorldToBlock(pos.x, pos.y);
	}

	static glm::ivec2 WorldToBlock(float x, float y)
	{
		return glm::ivec2(x > 0 ? (int)x : floor(x), y > 0 ? (int)y : floor(y));
	}
	//later check in all of these if block in world constraints (when implement looping)
	static glm::ivec2 BlockToChunk(int x, int y)
	{
		return glm::ivec2(floor((float)x / CHUNK_WIDTH), floor((float)y / CHUNK_HEIGHT));
	}

	static glm::ivec2 LocalToBlock(int x, int y, glm::ivec2 chunk)
	{
		return glm::ivec2(x + chunk.x * CHUNK_WIDTH, y + chunk.y * CHUNK_HEIGHT);
	};

	static glm::ivec2 BlockToLocal(int x, int y)
	{
		auto chunk = BlockToChunk(x, y);
		return glm::ivec2(x - chunk.x * CHUNK_WIDTH, y - chunk.y * CHUNK_HEIGHT);
	}

	glm::vec2 CollideLayer(const std::tuple<glm::vec3, glm::vec3> &ray) const;

	bool BoxCollide(const std::array<glm::vec2,4> box) const;

	unsigned int GetBlock(glm::ivec2 block, int layer)
	{
		return GetBlock(block.x, block.y, layer);
	}

	unsigned int GetBlock(int x, int y, int layer);

	void SetBlock(glm::ivec2 block, int layer, unsigned int id)
	{
		SetBlock(block.x, block.y, layer, id);
	}

	void SetBlock(int x, int y, int layer, unsigned int id);

	void PlaceBlock(glm::ivec2 block, unsigned int id)
	{
		PlaceBlock(block.x, block.y, id);
	}

	void PlaceBlock(int x, int y, unsigned int id);

	void RemoveBlock(glm::ivec2 block)
	{
		RemoveBlock(block.x, block.y);
	}

	void RemoveBlock(int x, int y);

	void RemoveBack(glm::ivec2 block)
	{
		RemoveBack(block.x, block.y);
	}

	void RemoveBack(int x, int y);

	std::shared_ptr<ChunkData> GetOrGenerateChunkData(glm::ivec2 chunk);

	void Render();

private:
	std::unordered_map<glm::ivec2, std::shared_ptr<ChunkData>> chunkData;
	std::unordered_map<glm::ivec2, std::shared_ptr<ChunkRenderer>> chunkRenderers;
	std::vector<glm::ivec2> chunksToUpdate;
};