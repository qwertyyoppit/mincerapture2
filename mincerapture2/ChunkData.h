#pragma once
#include <glm/glm.hpp>
#include "Constants.h"

class ChunkData
{
public:
	ChunkData(glm::ivec2 chunkPos) : ChunkPos(chunkPos)
	{
	}

	void SetBlock(glm::ivec2 pos, int layer, unsigned int block)
	{
		SetBlock(pos.x, pos.y, layer, block);
	}

	void SetBlock(int x, int y, int layer, unsigned int block)
	{
		backdata[x][y][layer] = block;
	}

	unsigned int GetBlock(glm::ivec2 pos, int layer) const
	{
		return GetBlock(pos.x, pos.y, layer);
	}

	unsigned int GetBlock(int x, int y, int layer) const
	{
		return backdata[x][y][layer];
	}

	glm::ivec2 ChunkPos{ 0 };

	unsigned short int backdata[CHUNK_WIDTH][CHUNK_HEIGHT][2] = {};
};