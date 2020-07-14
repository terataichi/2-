#pragma once
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 6
#define STAGE_Y 13

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;					// ステージID書き込み用
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	void Draw(void);									// 各スクリーンに描画する
	void UpDate(void);									// 更新.
private:
	void Init();
	Vector2 _offSet;									// ステージ画面のオフセット
	Vector2 _size;
	int _stageID;										// ステージの描画用スクリーンID

	std::vector<int> _dataBase;							// ステージ全体のマス管理用
	std::vector<int*> _data;							// dataBaseのポインターを入れて

	std::map<INPUT_ID, bool> _moveFlg;					// 移動していいか。true : ロック

	std::shared_ptr<InputState> _input;					// キーの入力管理

	std::shared_ptr<puyo> _puyo;

	static int playCnt;									// 複数人いた場合人数でｷｰを変えれるように
	int _id;
};

