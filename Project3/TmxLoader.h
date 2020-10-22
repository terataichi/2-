#pragma once
#include <map>

// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

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
	int width,
		height,
		tileWidth,
		tileHeight;
};

using LayerVec = std::vector<LayerData>;

class TmxLoader
{
public:
	TmxLoader();
	~TmxLoader();

	bool LoadTmx(std::string fileName);
	//rapidxml::xml_node<>& GetNode(std::string nodeName);
	LayerVec GetLayerData(void);
	MapData GetMapData(void);
private:
	bool LoadTsx(std::string fileName);

	std::string version_;											// 現在のファイルのVersionを定義して格納しておく
	std::string tsxFileName_;										// Tsxファイル名保存
	std::string ImageName_;
	LayerVec layerData_;
	MapData mapData_;
};

