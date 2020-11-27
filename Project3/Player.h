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

using MoveFuncInput = std::function<bool(TrgData, bool)>;

using MoveFuncAuto = std::function<bool(DIR, bool)>;

//ホストdef
class Player:
	public Object
{
public:
	Player(Vector2& pos,BaseScene& baseScene,LayerVec& layer);

	~Player();

	bool Update()override;
	void Draw(void)override;
	
	bool CheckWallAuto(void);
	bool CheckWallInput(DIR dir);
	bool CheckMoveBombAuto(void);
	bool CheckMoveBomb(DIR dir);									// ボムすり抜け防止

	bool UpdateDef()override;										// 入力処理管理
	bool UpdateAuto()override;										// オートパイロット
	bool UpdateRev()override;										// 受信

	void AddBombList(int no);
	int CheckBomb();

	static int lostCont_;

private:

	void InitFunc(void);											// ファンクションの初期化

	std::list<int> bombList_;										// 使えるボムリスト
	std::map<DIR, Vector2> dirMap_;									// キーが向き

	std::array<DIR, 5> nextDir_;									// 次のDIRを格納
	int dirCnt_;

	std::function<bool(void)>netFunc_;								// 自分が送信する側なのかどうかで処理が変わる

	std::unique_ptr<InputState> input_;

	BaseScene& scene_;												// ゲームシーンアクセス用

	std::list<MoveFuncInput>inputMoveList_;

	std::list<MoveFuncAuto>autoMoveList_;

	LayerVec& layerData_;

	int length_;													// ボムに情報を送るのに必要
};

