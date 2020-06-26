#pragma once
#include "Vector2.h"


class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);					// �N���b�N�����ꍇ�Atrue
	bool GetClicking(void);					// �N���b�N���̏ꍇ�Atrue
	const Vector2& GetPos(void);			// �د��ʒu�̎擾
	void Update(void);
private:
	Vector2 pos;
	char mouseData;		// ���݂̃N���b�N���
	char mouseDataOld;	// 1�ڰёO�̃}�E�X�N���b�N���
};

