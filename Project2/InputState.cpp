#include "InputState.h"

InputState::InputState()
{
}

InputState::~InputState()
{
}

const KeyPair& InputState::state(INPUT_ID id) const
{
	return keyState.at(id);
}
