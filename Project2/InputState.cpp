#include <DxLib.h>
#include "InputState.h"

InputState::InputState()
{
}

InputState::~InputState()
{
}

const KeyPair& InputState::state(INPUT_ID id) const
{
	return _keyState.at(id);
}

bool InputState::state(INPUT_ID id, int data)
{
	if (_keyState.find(id) != _keyState.end())
	{
		_keyState[id].first = data;
		return true;
	}
	return false;
}

void InputState::SetOld(void)
{
	for (auto id : INPUT_ID())
	{
		_keyState[id].second = _keyState[id].first;
	}
}
