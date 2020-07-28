#include <DxLib.h>
#include "MouseState.h"
#include "../_debug/_DebugConOut.h"
CntType MouseState::GetCntType(void)
{
	return CntType::Pad;
}

bool MouseState::SetUp(int no)
{
	_keyConDef = {
		//{INPUT_ID::BUTTON_LEFT,MOUSE_INPUT_LEFT},
		//{INPUT_ID::BUTTON_UP,PAD_INPUT_UP},
		//{INPUT_ID::BUTTON_RIGHT,MOUSE_INPUT_RIGHT},
		{INPUT_ID::BUTTON_ROTA_L,MOUSE_INPUT_RIGHT},
		{INPUT_ID::BUTTON_ROTA_L,MOUSE_INPUT_LEFT},
	};
	//_wheel = 0;
	//_oldwheel = 0;
	_keyCon = _keyConDef;
	GetMousePoint(&_sPos.x, &_sPos.y);
	return true;
}

void MouseState::UpDate(void)
{

	for (auto id : INPUT_ID())
	{
		_state[id][static_cast<int>(Trg::Old)] = _state[id][static_cast<int>(Trg::Now)];						// old�̃Z�b�g
		_state[id][static_cast<int>(Trg::Now)] = (GetMouseInput() & _keyCon[id]);								// now�̃Z�b�g
	}

	GetMousePoint(&_pos.x, &_pos.y);
	
	if (_sPos.x - _pos.x > 64)
	{
		_state[INPUT_ID::BUTTON_RIGHT][static_cast<int>(Trg::Now)] = true;										// now�̃Z�b�g
		GetMousePoint(&_sPos.x, &_sPos.y);
	}
	if (_sPos.x - _pos.x < -64)
	{
		_state[INPUT_ID::BUTTON_LEFT][static_cast<int>(Trg::Now)] = true;										// now�̃Z�b�g
		GetMousePoint(&_sPos.x, &_sPos.y);
	}
}