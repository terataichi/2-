#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Vector2.h"
#include "../DIR.h"
#include "../Scene/SceneMng.h"

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class TileMap;

/// <summary>
/// 炎の爆風生成用
/// </summary>
struct FlameGenerator
{
public:
	FlameGenerator(int length, Vector2& pos, TileMap& map);
	~FlameGenerator();
	bool Update(void);
private:
	bool CheckNextMap(void);
	int length_;
	Vector2 chipPos_;
	chronoTime time_;
	int count_;
	int dirCount_[4];
	TileMap& tileMap_;
};

struct dirBit
{
	int left : 8;
	int right : 8;
	int up : 8;
	int down : 8;
};

struct FlameMapData
{
	chronoTime startTime;
	dirBit dir{};
	bool next{};
};

using sharedFlame = std::shared_ptr<FlameGenerator>;

using FlameMapVec = std::vector<FlameMapData>;

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);					// TMXファイル読み込み
	bool SendTmxSizeData(void);							// TMXファイルのサイズを取得して送信
	bool SendTmxData(void);								// TMXファイルのデータ送信
	void Update(void);									// 描画の更新
	LayerVec& GetLayerVec(void);
	LayerData& GetLayerData(std::string name);			// レイヤーの情報取得
	MapData& GetMapData(void);
	const FlameMapVec& GetFlameMap(void)const;			// 当たり判定に使う
	std::vector<Vector2> GetCharChipPos();				// キャラクターの初期配置取得
	std::vector<Vector2> GetItemChipPos();				// キャラクターの初期配置取得

	bool SetFlameMap(dirBit dir,Vector2 size,bool next, chronoTime time);	// 爆発情報書き込み用

	void SetBombMap(int chipPos, bool flg);
	const std::vector<bool>GetBombMap(void)const;

	void AddFlameGenerate(int& length, Vector2& pos);	// 爆風生成
private:
	bool DrawMap(LayerData layerData);					// マップの描画

	// 読み込んだTMXファイルの情報保存用
	TmxLoader loader_;
	LayerVec layerVec_;
	MapData mapData_;
	std::string chipImageName_;

	std::vector<FlameMapData> flameMap_;				// 炎書き込み用マップ
	chronoTime oldTime_;
	std::list<sharedFlame> flameList_;					// 爆風リスト
	std::vector<bool>bombMap_;								// ボム用ﾏｯﾌﾟ							
};

