#include "WorldGen.h"
#include <iostream>
#include "BlockIDs.h"

WorldGen::WorldGen()
{
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			GenerateChunkData({ x, y });
		}
	}
	std::cout << "chunks generated!" << std::endl;
}

void WorldGen::GenerateChunkData(glm::ivec2 chunk)
{
	if (chunkData.find(chunk) == chunkData.end()) chunkData[chunk] = std::make_shared<ChunkData>(chunk);

	auto &data = chunkData[chunk];
	for (int layer = 0; layer < 2; layer++)
	{
		for (int x = 0; x < CHUNK_WIDTH; x++)
		{
			int worldx = x + chunk.x * CHUNK_WIDTH;
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int worldy = y + chunk.y * CHUNK_HEIGHT;
				if (worldy <= 30)
				{
					if (worldx == 0) 
					{
						data->SetBlock(x, y, layer, BlockID::Stone);
						continue;
					}
					if (worldy == 30)
					{
						data->SetBlock(x, y, layer, BlockID::Grass);
					}
					else if (worldy < 30 && worldy > 26)
					{
						data->SetBlock(x, y, layer, BlockID::Dirt);
					}
					else if (worldy < 27)
					{
						data->SetBlock(x, y, layer, BlockID::Stone);
					}
				}
			}
		}
	}
}

glm::vec2 WorldGen::CollideLayer(const std::tuple<glm::vec3, glm::vec3> &ray) const
{
	float dist = (get<0>(ray).z - WORLD_FRONT_Z) / fabs(get<1>(ray).z);
	return get<1>(ray) * dist + get<0>(ray);
}

unsigned int WorldGen::GetBlock(int x, int y, int layer)
{
	auto chunk = BlockToChunk(x, y);
	auto local = BlockToLocal(x, y);
	auto cd = GetOrGenerateChunkData(chunk);
	return cd->GetBlock(local, layer);
}

void WorldGen::SetBlock(int x, int y, int layer, unsigned int id)
{
	auto chunk = BlockToChunk(x, y);
	auto cd = GetOrGenerateChunkData(chunk);
	auto local = BlockToLocal(x, y);
	if (cd->GetBlock(local, layer) == id && layer == 1) layer = 0;
	cd->SetBlock(local, layer, id);
	for (int i = 0; i < 2; i++)
	{
		if (std::find(chunksToUpdate.begin(), chunksToUpdate.end(), chunk) == chunksToUpdate.end())
			chunksToUpdate.push_back(chunk);
		if (local.x == 0) chunk = { chunk.x - 1, chunk.y };
		else if (local.x == CHUNK_WIDTH - 1) chunk = { chunk.x + 1, chunk.y };
		else if (local.y == 0) chunk = { chunk.x, chunk.y - 1 };
		else if (local.y == CHUNK_HEIGHT - 1) chunk = { chunk.x, chunk.y + 1 };
		else return;
	}
}

void WorldGen::PlaceBlock(int x, int y, unsigned int id)
{
	if (GetBlock(x, y, 1)) return;

	SetBlock(x, y, 1, id);
	if (GetBlock(x, y, 0)) return;
	SetBlock(x, y, 0, id);

	int maxdown = 15;
	for (int down = 1; down < maxdown; down++)
	{
		if (GetBlock(x, y - down, 0))
		{
			for (int d = 1; d < down; d++)
			{
				SetBlock(x, y - d, 0, id);
			}
			return;
		}
	}
}

void WorldGen::RemoveBlock(int x, int y)
{
	if (!GetBlock(x, y, 1)) return;

	SetBlock(x, y, 1, 0);
	if (GetBlock(x, y + 1, 0)) return;

	SetBlock(x, y, 0, 0);
	int maxdown = 15;
	for (int down = 1; down < maxdown; down++)
	{
		if (GetBlock(x, y - down, 1)) return;
		else if (!GetBlock(x, y - down, 0)) return;
		SetBlock(x, y - down, 0, 0);
	}
}

void WorldGen::RemoveBack(int x, int y)
{
	if (GetBlock(x, y, 1)) return;
	//check for natural layer 0 blockdata property

	SetBlock(x, y, 0, 0);
}

std::shared_ptr<ChunkData> WorldGen::GetOrGenerateChunkData(glm::ivec2 chunk)
{
	if (chunkData.find(chunk) == chunkData.end()) GenerateChunkData(chunk);

	return chunkData.at(chunk);
}

void WorldGen::Render()
{
	//verify all chunkrenderers exist for chunks to render, create new ones if not
	// when creating, pass the corresponding current chunk (and later the chunks left, right, above, and below)
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			auto cd = GetOrGenerateChunkData({ x, y });
			
			if (chunkRenderers.find({ x, y }) == chunkRenderers.end())
			{
				chunkRenderers[{x, y}] = std::make_shared<ChunkRenderer>(cd);
				auto& cr = chunkRenderers[{x, y}];
				cr->GenerateChunkMesh();
			}
		}
	}
	for (auto chunk : chunksToUpdate)
	{
		if (chunkRenderers.find(chunk) == chunkRenderers.end())
		{
			continue;
		}
		auto& cr = chunkRenderers[chunk];
		cr->GenerateChunkMesh();
	}
	chunksToUpdate.clear();
	//call render on all chunk renderers
	for (int x = -2; x < 3; x++)
	{
		for (int y = -2; y < 3; y++)
		{
			auto &cr = chunkRenderers[{x, y}];
			cr->Render();
		}
	}
}
