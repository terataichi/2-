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

enum class PuyoID
{
	Non,
	red,
	Green,
	Bule,
	Yellow,
	Purple,
	Wall,
	Max,
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
	const Vector2& pos(void)const;
	const int size(void)const;
	const int rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// ���݂̃}�X�ڂ̎擾			
private:
	void Init(void);

	const int size_;							// �Ղ�̃T�C�Y
	const int rad_;								// �Ղ�̔��a
	Vector2 pos_;
	DirUnion dirFlg_;							// �ړ����Ă����̂��ǂ����̏����Z�b�g����
	PuyoID id_;									// �����̂Ղ�̎��ʗp
};

