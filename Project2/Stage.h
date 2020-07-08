#pragma once
#include <memory>
#include "common/Vector2.h"
#include "InputState.h"
#include "puyo.h"

class Stage
{
public:
	Stage();
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

	std::shared_ptr<InputState> _input;					// キーの入力管理

	std::shared_ptr<puyo> _puyo;
};

