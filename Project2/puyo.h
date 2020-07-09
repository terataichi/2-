#pragma once
#include "common/Vector2.h"

class puyo
{
public:
	puyo();
	~puyo();
	void UpDate(void);
	void Move(const Vector2& vec);
	void Draw(void);
private:
	bool IsCheckPos(void);			// �X�e�[�W�̊O�ɏo�邩�`�F�b�N����
	void Init(void);

	const Vector2 _size;			// �Ղ�̃T�C�Y
	const Vector2 _rad;				// �Ղ�̔��a
	Vector2 _pos;
};

