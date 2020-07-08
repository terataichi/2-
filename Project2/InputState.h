#pragma once
#include <vector>
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

using KeyTuple = std::tuple<int, int, int, int, int>;

class InputState
{
public:
	InputState();
	~InputState();
	void UpDate(void);
	const KeyPair& state(INPUT_ID id)const;		// キー情報を返す
	bool state(INPUT_ID id, int data);			// newセット関数
	void SetOld(void);							// oldセット関数
private:
	char _buf[256];								// キーボード情報取得用
	std::vector<int> _keyCon;					// キーの登録
	std::vector<int> _keyConDef;				//

	KeyMap _keyState;							// それぞれのkeyのnew と oldの情報格納

	static int playCnt;							// 複数人いた場合人数でｷｰを変えれるように
};

