#include "PadState.h"
#include <DxLib.h>

CntType PadState::GetCntType(void)
{
	return CntType::Pad;
}

bool PadState::SetUp(int no)
{
	if (no == 0)
	{
		_padID = DX_INPUT_PAD1;
	}
	if (no == 1)
	{
		_padID = DX_INPUT_PAD2;
	}
	_keyConDef = {
	{INPUT_ID::BUTTON_LEFT,PAD_INPUT_LEFT},
	{INPUT_ID::BUTTON_UP,PAD_INPUT_UP},
	{INPUT_ID::BUTTON_RIGHT,PAD_INPUT_RIGHT},
	{INPUT_ID::BUTTON_DOWN,PAD_INPUT_DOWN},
	{INPUT_ID::BUTTON_ROTA,PAD_INPUT_1},
	};

	_keyCon = _keyConDef;
	return true;
}

void PadState::UpDate(void)
{

	for (auto id : INPUT_ID())				// now�̃Z�b�g
	{
		_state[id][static_cast<int>(Trg::Old)] = _state[id][static_cast<int>(Trg::Now)];		// old�̃Z�b�g
		_state[id][static_cast<int>(Trg::Now)] = (GetJoypadInputState(_padID) & _keyCon[id]);								// now�̃Z�b�g
	}
}