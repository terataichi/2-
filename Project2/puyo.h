#pragma once
#include <map>
#include "common/Vector2.h"
#include "Input/INPUT_ID.h"

enum class PuyoState
{
	Drop,		// �����Ă�
	Stop,		// �~�܂�
	Die,		// ������
	Max
};

// �����̓��͂̃r�b�g�t�B�[���h
struct DirBits
{
	unsigned int left : 1;			// ��
	unsigned int up : 1;			// ��
	unsigned int right : 1;			// �E
	unsigned int down : 1;			// ��
};

union DirUnion
{
	DirBits bit;
	unsigned int dirFlg;
};

class puyo
{
public:
	puyo();
	~puyo();
	void UpDate(void);
	void Move(INPUT_ID id);
	void Draw(void);
	
	bool SetDirFlg(DirUnion flg);
	// ------�Q�b�g�֐�
	const Vector2 pos(void)const;
	const Vector2 size(void)const;
	const Vector2 rad(void)const;
private:
	void Init(void);

	const Vector2 _size;				// �Ղ�̃T�C�Y
	const Vector2 _rad;					// �Ղ�̔��a
	std::map<INPUT_ID, Vector2> _vec;	// �L�[�ɑ΂��Ă̈ړ��ʊi�[
	Vector2 _pos;
	DirUnion _dirFlg;					// �ړ����Ă����̂��ǂ����̏����Z�b�g����
};

