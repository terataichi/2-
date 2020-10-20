#include "TileMap.h"
#include <sstream>
#include <fstream>
#include "../_debug/_DebugConOut.h"

std::unique_ptr<TileMap, TileMap::TileMapDeleter> TileMap::sInstance_(new TileMap());

bool TileMap::LoadTileMap(std::string TileName)
{

	std::ifstream ifs("TileMap/" + TileName + ".tmx");
	if (ifs.fail())
	{
		TRACE("tmx ファイルが開けません\n");
		return false;
	}
	std::string data;
	int cnt = 0;

	// すべての行を読み取る
	while (true)
	{
		getline(ifs, data);

		cnt++;

		if (ifs.eof())break;
	}

	std::istringstream iss{ data[1] };

	std::string mapData;
	auto GetData = [&]()
	{
		getline(iss, mapData, ' ');
	};



	return true;
}

TileMap::TileMap()
{


}

TileMap::~TileMap()
{
}
