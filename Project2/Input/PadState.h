#pragma once
#include "InputState.h"
class PadState :
	public InputState
{
public:
	virtual CntType GetCntType(void) override;			// 自分がいま何を使っているのかを返す
	virtual bool SetUp(int no) override;				// 初期化
	virtual void Update(void) override;

	PadState();
	~PadState();
private:
	std::map<INPUT_ID, int> _keyCon;					// キーの登録
	std::map<INPUT_ID, int> _keyConDef;					//
	int _padID;
};

