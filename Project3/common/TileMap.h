#pragma once
#include <vector>
#include <map>
#include <string>

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"


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

using LayerVec = std::vector<LayerData>;

class TileMap
{
public:
	TileMap();
	~TileMap();

	bool LoadTmx(std::string file);


	LayerVec GetLayerData(void);
	MapData GetMapData(void);
private:
	LayerVec layer_;
	MapData mapData_;
	TmxLoader loader_;
};

