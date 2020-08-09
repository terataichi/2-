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
	Ojama,
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
	puyo(Vector2&& pos,PuyoID id) ;
	~puyo();
	virtual bool UpDate(int no);
	void Move(INPUT_ID id);										// �ړ��֐�
	void SoftDrop();											// �\�t�g�h���b�v
	bool CheckPuyon(void);										// �܂��Ղ�񂵂Ă邩�m�F����
	bool CheckMunyon(void);										// �ނɂ��J�E���g�`�F�b�N�[
	void Draw(void);											// �`��
	
	// ------ �Z�b�g�֐�
	bool SetDirFlg(DirUnion flg);								// �ړ��t���O�Z�b�g�p
	bool SetOldDirFlg(void);									// �ړ��t���O�̑O�̏����i�[
	bool SetMnyonFlg(DirUnion flg);								// �ނɂ�񂵂Ă������̏����Z�b�g
	void alive(bool flg);
	void SetCnt(int cnt);										// ���̕����[��
	void SetSpeed(int spped, int interval);						// �A���Ƃ��ŃX�s�[�h��ς���
	void SetPuyon(void);										// �Ă񂾂�Ղ��J�n
	void SetMunyon(int cnt);									// �Ă񂾂�ނɂ��J�n
	void pos(Vector2& vec);										// ���W
	void UpSpeed();												// ����������
	virtual void SetFall(bool flg);								// ���ז��p�A�����Ă������ǂ���
	// ------�Q�b�g�֐�
	virtual bool seiretu(void);									// ���ז��p�A����p
	const Vector2& pos(void)const;
	const DirUnion& GetDirFlg(void)const;
	const DirUnion& GetOldDirFlg(void)const;
	const int size(void)const;
	const Vector2 rad(void)const;
	const PuyoID id(void)const;
	const Vector2 GetGrid(int size);							// ���݂̃}�X�ڂ̎擾
	const Vector2 GetGrid(Vector2 pos, int size);				// �w�肵���ꏊ�̃}�X�ڂ̎擾
	const bool alive(void)const;
protected:
	void Init(Vector2& pos,PuyoID id);

	const int size_;											// �Ղ�̃T�C�Y
	const Vector2 rad_;											// �Ղ�̔��a
	Vector2 pos_;
	DirUnion dirFlg_;											// �ړ����Ă����̂��ǂ����̏����Z�b�g����
	DirUnion oldDirFlg_;										// �O�̏��
	DirUnion mnyonFlg_;											// �ނɂ��t���O�Atrue�������牡�ɐL�΂�
	PuyoID id_;													// �����̂Ղ�̎��ʗp
	int softCnt_;												// ���������p
	int softCntMax_;											// ���������̍ő�
	bool alive_;												// �����Ă邩�ǂ���
	int speed_;													// �Ղ�̑���
	
	bool speedUp_;

	int puyonOffset_;											// �Ղ�񂷂�Ƃ��̐U�ꕝ
	int puyonCnt_;												// �Ղ��J�E���g

	int munyonCnt_;												// �ނɂ��J�E���g

	std::map<PuyoID, int> puyoCor_;
};

