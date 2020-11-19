#pragma once
#include <map>
#include <memory>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "common/TileMap.h"
#include "Input/InputState.h"
#include "DIR.h"
#include "Object.h"
#include "Scene/BaseScene.h"

//ホストdef
class Player:
	public Object
{
public:
	Player(Vector2& pos,BaseScene& baseScene);

	~Player();

	bool Update(LayerVec&& layer)override;
	void Draw(void)override;
	
	bool CheckWall(LayerVec& layer);

	bool UpdateDef(LayerVec& layer)override;						// 入力処理管理
	bool UpdateAuto(LayerVec& layer)override;						// オートパイロット
	bool UpdateRev(LayerVec& layer)override;						// 受信

	void AddBombList(int no);
	int CheckBomb();

	static int lostCont_;

private:
	bool WallAuto(LayerData& layer);								// オートパイロット用壁判定
	bool WallInput(LayerData& layer);								// 操作用壁判定

	DIR dir_;

	std::list<int> bombList_;										// 使えるボムリスト
	std::map<DIR, Vector2> dirMap_;									// キーが向きで移動量が入る

	std::function<bool(LayerVec&)>netFunc_;							// 自分が送信する側なのかどうかで処理が変わる

	std::function<bool(LayerData&)>wallFunc_;						// 操作するかオートでするか

	std::unique_ptr<InputState> input_;

	BaseScene& scene_;												// ゲームシーンアクセス用

	int animCnt_;
};

