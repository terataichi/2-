#pragma once
#include "InputState.h"
#include "../common/Vector2.h"
class MouseState :
    public InputState
{
public:
	virtual CntType GetCntType(void) override;			// 自分がいま何を使っているのかを返す
	virtual bool SetUp(int no) override;				// 初期化
	virtual void UpDate(void) override;

private:
	std::map<INPUT_ID, int> _keyCon;					// キーの登録
	std::map<INPUT_ID, int> _keyConDef;					// デフォルト

	Vector2 _pos;										// マウスの座標
	Vector2 _sPos;										// 基準になる座標
};

