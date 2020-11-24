#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Vector2.h"

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class FlameGenerator;

using sharedFlame = std::shared_ptr<FlameGenerator>;

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);					// TMXファイル読み込み
	bool SendTmxSizeData(void);							// TMXファイルのサイズを取得して送信
	bool SendTmxData(void);								// TMXファイルのデータ送信
	void Update(void);								// 描画の更新
	LayerVec& GetLayerVec(void);
	LayerData& GetLayerData(std::string name);			// レイヤーの情報取得
	MapData& GetMapData(void);

	std::vector<Vector2> GetCharChipPos();				// キャラクターの初期配置取得

	void AddFlameGenerate(int& length, Vector2& pos);		// 爆風生成
private:
	bool DrawMap(LayerData layerData);					// マップの描画

	// 読み込んだTMXファイルの情報保存用
	TmxLoader loader_;
	LayerVec layerVec_;
	MapData mapData_;
	std::string chipImageName_;

	std::vector<int> flameMap_;							// 炎書き込み用マップ

	std::list<sharedFlame> flameList_;					// 爆風リスト
};

