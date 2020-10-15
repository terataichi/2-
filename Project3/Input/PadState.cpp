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
	_keyConDef.clear();
	_keyConDef = {
	{INPUT_ID::BUTTON_LEFT,PAD_INPUT_LEFT},
	{INPUT_ID::BUTTON_UP,PAD_INPUT_UP},
	{INPUT_ID::BUTTON_RIGHT,PAD_INPUT_RIGHT},
	{INPUT_ID::BUTTON_DOWN,PAD_INPUT_DOWN},
	{INPUT_ID::BUTTON_ROTA_L,PAD_INPUT_2},
	{INPUT_ID::BUTTON_ROTA_R,PAD_INPUT_1},
	{INPUT_ID::BUTTON_MOVEON,PAD_INPUT_4},
		//{INPUT_ID::BUTTON_MENU,},
	};

	for (auto id : INPUT_ID())				// nowのセット
	{
		_state[id][static_cast<int>(Trg::Old)] = 0;
		_state[id][static_cast<int>(Trg::Now)] = 0;
	}
	_keyCon = _keyConDef;
	return true;
}

void PadState::Update()
{

	for (auto id : INPUT_ID())				// nowのセット
	{
		_state[id][static_cast<int>(Trg::Old)] = _state[id][static_cast<int>(Trg::Now)];		// oldのセット
		_state[id][static_cast<int>(Trg::Now)] = (GetJoypadInputState(_padID) & _keyCon[id]);	// nowのセット
	}
}

PadState::PadState()
{
	_padID = 0;
}

PadState::~PadState()
{
}
