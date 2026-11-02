#include "InitBlockData.h"
#include "Sprite.h"
#include "BlockData.h"
#include "BlockRegistry.h"
#include "BlockIDs.h"
#include <functional>

void InitBlockData::Init()
{
	float unit = 1.f / (512.f / 16.f);
	Sprite grassTop({ unit, unit * 5, unit * 2, unit * 6 });
	Sprite grassSide({ 0.f, unit * 5, unit, unit * 6 });
	Sprite dirt({ 0.f, unit * 2, unit, unit * 3 });
	Sprite stone({ 0.f, unit * 1, unit * 1, unit * 2 });
	Sprite minedStone({ unit, unit * 1, unit * 2, unit * 2 });
	
	BlockData grassData(grassSide.uv, grassTop.uv, dirt.uv);
	BlockData dirtData(dirt.uv);
	BlockData stoneData(stone.uv);
	BlockData minedStoneData(minedStone.uv);

	BlockRegistry::RegisterBlockData(BlockID::Grass, grassData);
	BlockRegistry::RegisterBlockData(BlockID::Dirt, dirtData);
	BlockRegistry::RegisterBlockData(BlockID::Stone, stoneData);
	BlockRegistry::RegisterBlockData(BlockID::MinedStone, minedStoneData);

	std::function<void(int)> OnInteract = [](int a) 
		{
			a++;
		};
}