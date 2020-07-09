#pragma once
#include "InputState.h"
class KeyState :
	public InputState
{
public:
	KeyState(int id);
	~KeyState();
	void UpDate(void) override;
private:
	char _buf[256];								// �L�[�{�[�h���擾�p
	std::vector<int> _keyCon;					// �L�[�̓o�^
	std::vector<int> _keyConDef;				//
};

