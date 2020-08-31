#include "KeyState.h"
#include <DxLib.h>

CntType KeyState::GetCntType(void)
{
	return CntType::Key;
}

bool KeyState::SetUp(int no)
{
	_keyConDef.clear();
	if (no == 0)
	{
		_keyConDef = {
			{INPUT_ID::BUTTON_LEFT,KEY_INPUT_A},
			{INPUT_ID::BUTTON_UP,KEY_INPUT_W},
			{INPUT_ID::BUTTON_RIGHT,KEY_INPUT_D},
			{INPUT_ID::BUTTON_DOWN,KEY_INPUT_S},
			{INPUT_ID::BUTTON_ROTA_L,KEY_INPUT_Z},
			{INPUT_ID::BUTTON_ROTA_R,KEY_INPUT_X},
		};
	}
	if (no == 1)
	{
		_keyConDef = 
		{
			{INPUT_ID::BUTTON_LEFT,KEY_INPUT_LEFT},
			{INPUT_ID::BUTTON_UP,KEY_INPUT_UP},
			{INPUT_ID::BUTTON_RIGHT,KEY_INPUT_RIGHT},
			{INPUT_ID::BUTTON_DOWN,KEY_INPUT_DOWN},
			{INPUT_ID::BUTTON_ROTA_L,KEY_INPUT_NUMPAD0},
			{INPUT_ID::BUTTON_ROTA_R,KEY_INPUT_NUMPAD3},
		};
	}

	// 共通
	_keyConDef.try_emplace(INPUT_ID::BUTTON_MENU, KEY_INPUT_M);
	_keyConDef.try_emplace(INPUT_ID::BUTTON_MOVEON, KEY_INPUT_1);
	_keyConDef.try_emplace(INPUT_ID::BUTTON_RETURN, KEY_INPUT_2);

	for (auto id : INPUT_ID())
	{
		_state[id][static_cast<int>(Trg::Now)] = 0;
		_state[id][static_cast<int>(Trg::Old)] = 0;
	}

	_keyCon = _keyConDef;
	return false;
}

void KeyState::Update()
{
	GetHitKeyStateAll(_buf.data());																// キーボードの情報取得

	for (auto id : INPUT_ID())
	{
		_state[id][static_cast<int>(Trg::Old)] = _state[id][static_cast<int>(Trg::Now)];		// oldのセット
		_state[id][static_cast<int>(Trg::Now)] = _buf[_keyCon[id]];								// nowのセット
	}
}

KeyState::KeyState()
{
}

KeyState::~KeyState()
{
}
