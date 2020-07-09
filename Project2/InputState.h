#pragma once
#include <vector>
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

class InputState
{
public:
	InputState();
	~InputState();
	virtual void UpDate(void) = 0;
	const KeyPair& state(INPUT_ID id)const;		// �L�[����Ԃ�
	bool state(INPUT_ID id, int data);			// new�Z�b�g�֐�
	void SetOld(void);							// old�Z�b�g�֐�
private:

	KeyMap _keyState;							// ���ꂼ���key��new �� old�̏��i�[
};

