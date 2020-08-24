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
#define STAGE_Y 16

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

// 勝ち負け判断用
enum class Victory
{
	Non,
	Win,
	Lose,
	Draw
};

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	void DrawUpdate(void);												// 各スクリーンに描画する
	void DrawStage(void);
	int  Update(int ojamaCnt);											// 更新.

	// ------ Set
	bool SetErase(SharePuyo& puyo, Vector2 vec);						// ぷよを消すのをセット
	void ResetRensa(void);												// 連鎖が終わるときに使う
	void AddRensa(void);												// 連鎖を増やす
	void ojamaCnt(int cnt);												// お邪魔プヨを相手に送り付けるときに使う
	void gameOverCnt(int cnt);											
	void victory(Victory vic);
	// ------ Get
	const int GetStageID(void)const;									// ステージID書き込み用
	const Vector2 offSet(void)const;									// ステージのオフセット
	const Vector2 size(void) const;										// サイズの取得
	const int rensa(void)const;											// 連鎖の取得
	const Vector2 GetGrid(Vector2 pos);									// 指定した座標のマス目を取得;]
	const int ojamaCnt(void)const;										// 自分のお邪魔ｶｳﾝﾄ取得
	const int id(void)const;											// すてーじのID取得
	const bool alive(void)const;										// 生きてるか死んでるか
	const bool nextScene(void)const;
	const Victory& victory(void)const;
private:

	void Init(void);
	bool CheckMove(SharePuyo& vec);										// 上下左右動いていいか
	void InstancePuyo();												// 新しいプヨ
	void InstanceOjama(int no);											// お邪魔プヨの生成
	Vector2 offSet_;													// ステージ画面のオフセット
	Vector2 size_;
	int blockSize_;
	int stageID_;														// ステージの描画用スクリーンID
	int puyoID_;														// ぷよの描画ID
	int guideID_;														// ガイド用スクリーン
	int hideID_;														// いちます上のところを隠すためのマスクを作成
	int ojamaPuyoID_;													// お邪魔プヨ描画用
	Vector2 gameOverPos_;												// ゲームオーバーの時に動かす用
	double angle_;														// ステージの角度

	int maxRensa_;														// 今までの最大連鎖
	int rensa_;															// 今何連鎖
	int ojamaCnt_;														// 何個お邪魔あるのー
	int eraseCnt_;														// 今回何個ぷよきえたのー
	int gameOverCnt_;													// ゲームオーバーになって

	bool ojamaFlg = false;

	std::unique_ptr<PlayUnit> playUnit_;								// ﾌﾟﾚｲﾔｰに関する処理をフレンドでもらって管理
	std::unique_ptr<NextPuyo> nextPuyo_;								// ネクストプヨ管理用

	std::vector<SharePuyo> dataBase_;									// ステージ全体のマス管理用
	std::vector<SharePuyo*> data_;										// dataBaseのポインターを入れて

	std::vector<SharePuyo> eraseDataBase_;								// ステージ全体のマスで４つ揃ったら消える
	std::vector<SharePuyo*> erasedata_;									// eraseDataBaseのポインターを入れて

	std::map<INPUT_ID, bool> moveFlg_;									// 移動していいか。true : ロック

	std::map<CntType,std::shared_ptr<InputState>> input_;				// キーの入力管理

	int inputNum = 0;													// ページアップページダウンで操作対象を切り替えたい

	std::vector<SharePuyo> puyoVec_;									// ぷよのリスト
	std::list<SharePuyo> ojamaList_;									// お邪魔リスト

	std::map<StgMode, std::function<void(Stage&)>> modeMap_;			// モード別関数オブジェクト
	std::map<Victory, std::function<bool(Stage&)>> victoryMap_;			// 勝ち負けで処理を変える

	static std::map<int, int> stgBG_;									// ステージによって背景を変えれるように

	bool alive_;														// ステージが生きてるか

	static int playCnt_;												// 複数人いた場合人数でキーを変えれるように
	int id_;															// ステージの番号
	StgMode stgMode_;
	Victory victory_;													// 勝ちなのか負けなのか引き分けなのか

	bool nextScene_;													// 次のシーンに移動していいかどうか

	friend class  PlayUnit;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct DropMode;
	friend struct MunyonMode;
	friend struct GameOver;
	friend struct DeleteMode;
	friend struct Draw;
	friend struct Lose;
	friend struct Win;

	//friend struct OjamaMode;
};

