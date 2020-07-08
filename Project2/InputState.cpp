#include <DxLib.h>
#include "InputState.h"

int InputState::playCnt = 1;

InputState::InputState()
{
	if (playCnt == 1)
	{
		_keyConDef.emplace_back(KEY_INPUT_A);
		_keyConDef.emplace_back(KEY_INPUT_W);
		_keyConDef.emplace_back(KEY_INPUT_D);
		_keyConDef.emplace_back(KEY_INPUT_S);
		_keyConDef.emplace_back(KEY_INPUT_Z);
	}
	if (playCnt == 2)
	{
		_keyConDef.emplace_back(KEY_INPUT_LEFT);
		_keyConDef.emplace_back(KEY_INPUT_UP);
		_keyConDef.emplace_back(KEY_INPUT_RIGHT);
		_keyConDef.emplace_back(KEY_INPUT_DOWN);
		_keyConDef.emplace_back(KEY_INPUT_RETURN);
	}

	_keyCon = _keyConDef;

	playCnt++;
}

InputState::~InputState()
{
}

void InputState::UpDate(void)
{
	SetOld();								// old�̐ݒ�
	GetHitKeyStateAll(_buf);				// �L�[�{�[�h�̏��擾

	for (auto id : INPUT_ID())				// now�̃Z�b�g
	{
		state(id, static_cast<int>(_buf[_keyCon[static_cast<int>(id)]]));
	}
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
