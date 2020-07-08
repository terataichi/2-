#pragma once
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;
class InputState
{
public:
	InputState();
	~InputState();
	virtual void UpDate() = 0;
	const KeyPair& state(INPUT_ID id)const;

private:

	KeyMap keyState;							// ‚»‚ê‚¼‚ê‚Ìkey‚Ìnew ‚Æ old‚Ìî•ñŠi”[
};

