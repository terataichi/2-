#pragma once
#include <map>
#include "common/Vector2.h"
#include "Input/INPUT_ID.h"

class puyo
{
public:
	puyo();
	~puyo();
	void UpDate(void);
	void Move(INPUT_ID id);
	void Draw(void);
	const Vector2 pos(void)const;
	const Vector2 size(void)const;
private:
	void Init(void);

	const Vector2 _size;				// �Ղ�̃T�C�Y
	const Vector2 _rad;					// �Ղ�̔��a
	std::map<INPUT_ID, Vector2> _vec;	// �L�[�ɑ΂��Ă̈ړ��ʊi�[
	Vector2 _pos;
};

