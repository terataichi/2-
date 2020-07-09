#include "PadState.h"
#include <DxLib.h>

PadState::PadState(int id)
{
	if (id == 1)
	{
		_padID = DX_INPUT_PAD1;
	}
	if (id == 2)
	{
		_padID = DX_INPUT_PAD2;
	}
	_keyConDef.emplace_back(PAD_INPUT_LEFT);
	_keyConDef.emplace_back(PAD_INPUT_UP);
	_keyConDef.emplace_back(PAD_INPUT_RIGHT);
	_keyConDef.emplace_back(PAD_INPUT_DOWN);
	_keyConDef.emplace_back(PAD_INPUT_DOWN);

	_keyCon = _keyConDef;
}

PadState::~PadState()
{
}

void PadState::UpDate(void)
{
	SetOld();								// oldÇÃê›íË

	for (auto id : INPUT_ID())				// nowÇÃÉZÉbÉg
	{
		state(id, static_cast<int>(	GetJoypadInputState(_padID)& _keyCon[static_cast<int>(id)]));
	}
}
