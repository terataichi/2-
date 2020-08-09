#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"
#include "OjamaPuyo.h"

#define STAGE_X 8
#define STAGE_Y 15

class PlayUnit;
class NextPuyo;

using SharePuyo = std::shared_ptr<puyo>;

enum class StgMode
{
	Drop,
	Puyon,
	Munyon,
	Rensa,
	FALL,
	Delete,
	Ojama,
	GameOver
};

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	void DrawUpdate(void);												// 各スクリーンに描画する
	void DrawStage(void);
	int  UpDate(int ojamaCnt);											// 更新.

	// ------ Set
	bool SetErase(SharePuyo& puyo, Vector2 vec);						// ぷよを消すのをセット
	void ResetRensa(void);												// 連鎖が終わるときに使う
	void AddRensa(void);												// 連鎖を増やす
	void ojamaCnt(int cnt);												// お邪魔プヨを相手に送り付けるときに使う
	// ------ Get
	const int GetStageID(void)const;									// ステージID書き込み用
	const Vector2 offSet(void)const;									// ステージのオフセット
	const Vector2 size(void) const;										// サイズの取得
	const int rensa(void)const;											// 連鎖の取得
	const Vector2 GetGrid(Vector2 pos);									// 指定した座標のマス目を取得;]
	const int ojamaCnt(void)const;										// 自分のお邪魔ｶｳﾝﾄ取得
	const int id(void)const;											// すてーじのID取得
private:

	void Init(void);
	bool CheckMove(SharePuyo& vec);										// 上下左右動いていいか
	void InstancePuyo();												// 新しいプヨ
	void InstanceOjama();												// お邪魔プヨの生成
	Vector2 offSet_;													// ステージ画面のオフセット
	Vector2 size_;
	int blockSize_;
	int stageID_;														// ステージの描画用スクリーンID

	int maxRensa_;														// 今までの最大連鎖
	int rensa_;															// 今何連鎖
	int ojamaCnt_;														// 何個お邪魔あるのー
	int eraseCnt_;														// 今回何個ぷよきえたのー

	std::unique_ptr<PlayUnit> playUnit_;								// ﾌﾟﾚｲﾔｰに関する処理をフレンドでもらって管理
	std::unique_ptr<NextPuyo> nextPuyo_;

	std::vector<SharePuyo> dataBase_;									// ステージ全体のマス管理用
	std::vector<SharePuyo*> data_;										// dataBaseのポインターを入れて

	std::vector<SharePuyo> eraseDataBase_;								// ステージ全体のマスで４つ揃ったら消える
	std::vector<SharePuyo*> erasedata_;									// eraseDataBaseのポインターを入れて

	std::map<INPUT_ID, bool> moveFlg_;									// 移動していいか。true : ロック

	std::shared_ptr<InputState> input_;									// キーの入力管理

	std::vector<SharePuyo> puyoVec_;									// ぷよのリスト
	std::list<SharePuyo> ojamaList_;									// お邪魔リスト

	std::map<StgMode, std::function<void(Stage&)>> modeMap_;			// モード別関数オブジェクト

	static int playCnt_;												// 複数人いた場合人数でキーを変えれるように
	int id_;
	StgMode stgMode_;

	friend class  PlayUnit;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct DropMode;
	friend struct MunyonMode;
	friend struct GameOver;
	friend struct DeleteMode;
	//friend struct OjamaMode;
};

