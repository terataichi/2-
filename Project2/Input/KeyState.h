#pragma once
#include <array>
#include <map>
#include "InputState.h"
#include "INPUT_ID.h"

struct KeyState :
	public InputState
{
public:
	virtual CntType GetCntType(void) override;			// 自分がいま何を使っているのかを返す
	virtual bool SetUp(int no) override;
	virtual void Update(void) override;

private:
	std::array<char, 256> _buf;							// キーボード情報取得用
	std::map<INPUT_ID, int> _keyCon;					// キーの登録
	std::map<INPUT_ID, int> _keyConDef;					//
};

