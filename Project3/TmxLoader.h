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
	std::string GetImageName(void);
	std::string GetTmxFileName(void);
private:
	bool LoadTsx(std::string fileName);

	std::string version_;											// 現在のファイルのVersionを定義して格納しておく
	std::string tmxFileName_;										// Tmxファイル名保存
	std::string tsxFileName_;										// Tsxファイル名保存
	std::string ImageName_;											// タイルの画像取得
	LayerVec layerData_;
	MapData mapData_;


	rapidxml::xml_document<> doc;
};

