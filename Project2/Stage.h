#pragma once
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 8
#define STAGE_Y 15

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;					// ステージID書き込み用
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	bool SetErase(void);								// ぷよを消すのをセット
	void Draw(void);									// 各スクリーンに描画する
	void UpDate(void);									// 更新.
private:
	void Init();
	Vector2 offSet_;									// ステージ画面のオフセット
	Vector2 size_;
	int blockSize_;
	int stageID_;										// ステージの描画用スクリーンID

	std::vector<PuyoID> dataBase_;						// ステージ全体のマス管理用
	std::vector<PuyoID*> data_;							// dataBaseのポインターを入れて

	std::vector<PuyoID> eraseDataBase_;					// ステージ全体のマス管理用
	std::vector<PuyoID*> erasedata_;					// dataBaseのポインターを入れて

	std::map<INPUT_ID, bool> moveFlg_;					// 移動していいか。true : ロック

	std::shared_ptr<InputState> input_;					// キーの入力管理

	std::vector<std::shared_ptr<puyo>> puyoVec_;

	static int playCnt_;								// 複数人いた場合人数でｷｰを変えれるように
	int id_;
};

