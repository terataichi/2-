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
	Red,
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
	puyo(Vector2&& pos,PuyoID id);
	~puyo();
	bool UpDate(void);
	void Move(INPUT_ID id);										// �ړ��֐�
	void SoftDrop();											// �\�t�g�h���b�v
	bool CheckPuyon();											// �܂��Ղ�񂵂Ă邩�m�F����
	void Draw(void);											// �`��
	
	// ------ �Z�b�g�֐�
	bool SetDirFlg(DirUnion flg);
	bool SetOldDirFlg(void);
	void alive(bool flg);
	void SetWidth(int width);									// ���̕����[��
	void SetSpeed(int spped, int interval);						// �A���Ƃ��ŃX�s�[�h��ς���
	void SetPuyon(void);										// �Ă񂾂�Ղ��J�n
	void SetMunyon(void);										// �Ă񂾂�ނɂ��J�n
	void pos(Vector2& vec);										// ���W
	void UpSpeed();												// ����������

	// ------�Q�b�g�֐�
	const Vector2& pos(void)const;
	const DirUnion& GetDirFlg(void)const;
	const DirUnion& GetOldDirFlg(void)const;
	const int size(void)const;
	const Vector2 rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// ���݂̃}�X�ڂ̎擾
	const Vector2 GetGrid(Vector2 pos, int size);				// �w�肵���ꏊ�̃}�X�ڂ̎擾
	const bool alive(void)const;
private:
	void Init(Vector2& pos,PuyoID id);

	const int size_;											// �Ղ�̃T�C�Y
	const Vector2 rad_;											// �Ղ�̔��a
	Vector2 pos_;
	DirUnion dirFlg_;											// �ړ����Ă����̂��ǂ����̏����Z�b�g����
	DirUnion oldDirFlg_;										// �O�̏��
	PuyoID id_;													// �����̂Ղ�̎��ʗp
	int softCnt_;												// ���������p
	int softCntMax_;											// ���������̍ő�
	bool alive_;												// �����Ă邩�ǂ���
	int speed_;													// �Ղ�̑���
	
	bool up_ = false;

	int width_;													// �Ղ�񂷂�Ƃ��̐U�ꕝ
	int puyonCnt_;												// �Ղ��J�E���g

	std::map<PuyoID, int> puyoCor_;
};

