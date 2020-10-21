#pragma once
#include <memory>
#include <string>

#define lpTileMap TileMap::GetInstance()

#define TMX_VERSION "1.4"

class TileMap
{
public:
	TileMap();
	~TileMap();

	bool LoadTmx(std::string file);

private:
	
	std::string version;
	std::string tiledversion;
	std::string orientation;
	std::string renderorder;			// enumとかに変えたい
	int width;							// タイルのマスの数(横)
	int height;							// 　　　 "　　　　(縦)
	int tileWidth;						// 1タイルのサイズ(横)
	int tileHeight;						//　　　 "　　　　(縦)
	int infinite;
	int nextLayerID;					// layerの数
	int nextObjectID;
};

