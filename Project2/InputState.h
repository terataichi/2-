#pragma once
#include <vector>
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

class InputState
{
public:
	InputState();
	~InputState();
	virtual void UpDate(void) = 0;
	const KeyPair& state(INPUT_ID id)const;		// キー情報を返す
	bool state(INPUT_ID id, int data);			// newセット関数
	void SetOld(void);							// oldセット関数
private:

	KeyMap _keyState;							// それぞれのkeyのnew と oldの情報格納
};

