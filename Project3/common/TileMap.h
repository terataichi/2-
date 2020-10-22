#pragma once
#include <vector>
#include <map>
#include <string>

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);
	bool DrawUpdate(void);								// 描画の更新
	bool DrawMap(LayerData layerData);					// マップの描画
private:
	void Init(void);									// 初期化
	TmxLoader loader_;
	LayerVec layerData_;
	MapData mapData_;
};

