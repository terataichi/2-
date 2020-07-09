#include "KeyState.h"
#include <DxLib.h>

KeyState::KeyState(int id)
{
	if (id == 1)
	{
		_keyConDef.emplace_back(KEY_INPUT_A);
		_keyConDef.emplace_back(KEY_INPUT_W);
		_keyConDef.emplace_back(KEY_INPUT_D);
		_keyConDef.emplace_back(KEY_INPUT_S);
		_keyConDef.emplace_back(KEY_INPUT_Z);
	}
	if (id == 2)
	{
		_keyConDef.emplace_back(KEY_INPUT_LEFT);
		_keyConDef.emplace_back(KEY_INPUT_UP);
		_keyConDef.emplace_back(KEY_INPUT_RIGHT);
		_keyConDef.emplace_back(KEY_INPUT_DOWN);
		_keyConDef.emplace_back(KEY_INPUT_RETURN);
	}

	_keyCon = _keyConDef;
}

KeyState::~KeyState()
{
}

void KeyState::UpDate(void)
{
	SetOld();								// oldの設定
	GetHitKeyStateAll(_buf);				// キーボードの情報取得

	for (auto id : INPUT_ID())				// nowのセット
	{
		state(id, static_cast<int>(_buf[_keyCon[static_cast<int>(id)]]));
	}
}
