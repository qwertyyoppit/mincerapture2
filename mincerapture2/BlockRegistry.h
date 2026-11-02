#pragma once
#include <unordered_map>
#include "BlockData.h"

class BlockRegistry
{
public:
	static std::unordered_map<short, BlockData> BlockDatas;

	static void RegisterBlockData(short id, BlockData data);

	static BlockData& GetBlock(short id);

private:
	BlockRegistry();
};