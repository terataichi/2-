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
			{INPUT_ID::BUTTON_ATTACK,KEY_INPUT_Z},
			{INPUT_ID::BUTTON_ROTA_R,KEY_INPUT_X},
		};
	}
	if (no == 1)
	{
		_keyConDef = 
		{
			{INPUT_ID::BUTTON_NUM_0,KEY_INPUT_NUMPAD0},
			{INPUT_ID::BUTTON_NUM_1,KEY_INPUT_NUMPAD1},
			{INPUT_ID::BUTTON_NUM_2,KEY_INPUT_NUMPAD2},
			{INPUT_ID::BUTTON_NUM_3,KEY_INPUT_NUMPAD3},
			{INPUT_ID::BUTTON_NUM_4,KEY_INPUT_NUMPAD4},
			{INPUT_ID::BUTTON_NUM_5,KEY_INPUT_NUMPAD5},
			{INPUT_ID::BUTTON_NUM_6,KEY_INPUT_NUMPAD6},
			{INPUT_ID::BUTTON_NUM_7,KEY_INPUT_NUMPAD7},
			{INPUT_ID::BUTTON_NUM_8,KEY_INPUT_NUMPAD8},
			{INPUT_ID::BUTTON_NUM_9,KEY_INPUT_NUMPAD9},
			{INPUT_ID::BUTTON_NUM_DOT,KEY_INPUT_DECIMAL},
			{INPUT_ID::BUTTON_NUM_ENTER,KEY_INPUT_NUMPADENTER},
			{INPUT_ID::BUTTON_NUM_MINUS,KEY_INPUT_SUBTRACT},
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
