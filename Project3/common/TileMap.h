#pragma once
#include <vector>
#include <map>
#include <string>

#define TMX_VERSION "1.4"

using LayerMap = std::map<std::string, MapLoader::LayerData>;

namespace MapLoader
{
	struct LayerData
	{
		std::string name;
		std::vector<int>chipData;
		int id,
			width,
			heigth;
	};

	struct MapData
	{
		std::string 
			renderOrder;
		int width,
			height,
			tileWidth,
			tileHeight;
	};

	class TileMap
	{
	public:
		TileMap();
		~TileMap();

		bool LoadTmx(std::string file);

		LayerMap GetLayerData(void);
		MapData GetMapData(void);
	private:
		LayerMap layer_;
		MapData mapData_;
	};
}

