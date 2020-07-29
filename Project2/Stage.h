#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 8
#define STAGE_Y 15

class PlayUnit;

using SharePuyo = std::shared_ptr<puyo>;

enum class StgMode
{
	Drop,
	Puyon,
	Rensa,
	FALL
};

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;									// ステージID書き込み用
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	const Vector2 GetGrid(Vector2 pos);									// 指定した座標のマス目を取得;
	bool SetErase(SharePuyo& puyo, Vector2 vec);						// ぷよを消すのをセット
	void Draw(void);													// 各スクリーンに描画する
	void UpDate(void);													// 更新.
private:

	void Init(void);
	bool CheckMove(SharePuyo& vec);										// 上下左右動いていいか
	void InstancePuyo();												// 新しいプヨ
	Vector2 offSet_;													// ステージ画面のオフセット
	Vector2 size_;
	int blockSize_;
	int stageID_;														// ステージの描画用スクリーンID

	std::unique_ptr<PlayUnit> playUnit_;								// ﾌﾟﾚｲﾔｰに関する処理をフレンドでもらって管理

	std::vector<SharePuyo> dataBase_;									// ステージ全体のマス管理用
	std::vector<SharePuyo*> data_;										// dataBaseのポインターを入れて

	std::vector<SharePuyo> eraseDataBase_;								// ステージ全体のマスで４つ揃ったら消える
	std::vector<SharePuyo*> erasedata_;									// eraseDataBaseのポインターを入れて

	std::map<INPUT_ID, bool> moveFlg_;									// 移動していいか。true : ロック

	std::shared_ptr<InputState> input_;									// キーの入力管理

	std::vector<SharePuyo> puyoVec_;

	std::map<StgMode, std::function<void(Stage&)>> modeMap_;			// モード別関数オブジェクト

	static int playCnt_;												// 複数人いた場合人数でキーを変えれるように
	int id_;
	StgMode stgMode_;

	friend class  PlayUnit;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct DropMode;

};

