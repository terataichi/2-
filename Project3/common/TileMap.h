#pragma once
#include <vector>
#include <map>
#include <string>
#include "Vector2.h"

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);					// TMXファイル読み込み
	bool SendTmxSizeData(void);							// TMXファイルのサイズを取得して送信
	bool SendTmxData(void);								// TMXファイルのデータ送信
	void DrawUpdate(void);								// 描画の更新
	LayerVec GetLayerData(void);
	MapData GetMapData(void);

	std::vector<Vector2> GetCharChipPos();					// キャラクターの初期配置取得

private:
	bool DrawMap(LayerData layerData);					// マップの描画


	// 読み込んだTMXファイルの情報保存用
	TmxLoader loader_;
	LayerVec layerData_;
	MapData mapData_;
	std::string ImageName_;
};

