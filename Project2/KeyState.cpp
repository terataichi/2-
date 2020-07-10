#include "KeyState.h"
#include <DxLib.h>

CntType KeyState::GetCntType(void)
{
	return CntType::Key;
}

bool KeyState::SetUp(int no)
{
	if (no == 0)
	{
		_keyConDef = {
			{INPUT_ID::BUTTON_LEFT,KEY_INPUT_A},
			{INPUT_ID::BUTTON_UP,KEY_INPUT_W},
			{INPUT_ID::BUTTON_RIGHT,KEY_INPUT_D},
			{INPUT_ID::BUTTON_DOWN,KEY_INPUT_S},
			{INPUT_ID::BUTTON_ROTA,KEY_INPUT_Z},
		};
	}
	else if (no == 1)
	{
		_keyConDef = 
		{
			{INPUT_ID::BUTTON_LEFT,KEY_INPUT_LEFT},
			{INPUT_ID::BUTTON_UP,KEY_INPUT_UP},
			{INPUT_ID::BUTTON_RIGHT,KEY_INPUT_RIGHT},
			{INPUT_ID::BUTTON_DOWN,KEY_INPUT_DOWN},
			{INPUT_ID::BUTTON_ROTA,KEY_INPUT_RETURN},
		};
	}


	_keyCon = _keyConDef;
	return false;
}

void KeyState::UpDate(void)
{
	GetHitKeyStateAll(_buf.data());																// キーボードの情報取得

	for (auto id : INPUT_ID())
	{
		_state[id][static_cast<int>(Trg::Old)] = _state[id][static_cast<int>(Trg::Now)];		// oldのセット
		_state[id][static_cast<int>(Trg::Now)] = _buf[_keyCon[id]];								// nowのセット
	}
}
