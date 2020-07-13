#pragma once
#include "InputState.h"
#include "../common/Vector2.h"
class MouseState :
    public InputState
{
public:
	virtual CntType GetCntType(void) override;			// ���������܉����g���Ă���̂���Ԃ�
	virtual bool SetUp(int no) override;				// ������
	virtual void UpDate(void) override;

private:
	std::map<INPUT_ID, int> _keyCon;					// �L�[�̓o�^
	std::map<INPUT_ID, int> _keyConDef;					// �f�t�H���g

	Vector2 _pos;										// �}�E�X�̍��W
	Vector2 _sPos;										// ��ɂȂ���W
};

