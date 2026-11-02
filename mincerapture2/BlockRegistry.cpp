#include "BlockRegistry.h"
#include <iostream>
#include <string>

std::unordered_map<short, BlockData> BlockRegistry::BlockDatas;

void BlockRegistry::RegisterBlockData(short id, BlockData data)
{
	BlockDatas[id] = data;
}

BlockData& BlockRegistry::GetBlock(short id)
{
	try
	{
		return BlockDatas.at(id);
	}
	catch (const _exception& e)
	{
		std::string errmsg = "Block ID " + std::to_string(id) + " does not exist!";
		std::cout << errmsg;
		throw e;
	}
}