#pragma once
#include <array>
#include <map>
#include "InputState.h"
#include "INPUT_ID.h"

struct KeyState :
	public InputState
{
public:
	virtual CntType GetCntType(void) override;			// ���������܉����g���Ă���̂���Ԃ�
	virtual bool SetUp(int no) override;
	virtual void Update(void) override;

private:
	std::array<char, 256> _buf;							// �L�[�{�[�h���擾�p
	std::map<INPUT_ID, int> _keyCon;					// �L�[�̓o�^
	std::map<INPUT_ID, int> _keyConDef;					//
};

