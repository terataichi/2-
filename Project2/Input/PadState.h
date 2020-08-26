#pragma once
#include "InputState.h"
class PadState :
	public InputState
{
public:
	virtual CntType GetCntType(void) override;			// ���������܉����g���Ă���̂���Ԃ�
	virtual bool SetUp(int no) override;				// ������
	virtual void Update(void) override;

	PadState();
	~PadState();
private:
	std::map<INPUT_ID, int> _keyCon;					// �L�[�̓o�^
	std::map<INPUT_ID, int> _keyConDef;					//
	int _padID;
};

