#pragma once
#include "InputState.h"
class KeyState :
	public InputState
{
public:
	KeyState(int id);
	~KeyState();
	void UpDate(void) override;
private:
	char _buf[256];								// キーボード情報取得用
	std::vector<int> _keyCon;					// キーの登録
	std::vector<int> _keyConDef;				//
};

