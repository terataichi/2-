#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"
#include "OjamaPuyo.h"

#define STAGE_X 8
#define STAGE_Y 16

class PlayUnit;
class NextPuyo;

using SharePuyo = std::shared_ptr<puyo>;

enum class StgMode
{
	Drop,
	Puyon,
	Munyon,
	Rensa,
	FALL,
	Delete,
	Ojama,
	GameOver
};

// �����������f�p
enum class Victory
{
	Non,
	Win,
	Lose,
	Draw
};

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	void DrawUpdate(void);												// �e�X�N���[���ɕ`�悷��
	void DrawStage(void);
	int  Update(int ojamaCnt);											// �X�V.

	// ------ Set
	bool SetErase(SharePuyo& puyo, Vector2 vec);						// �Ղ�������̂��Z�b�g
	void ResetRensa(void);												// �A�����I���Ƃ��Ɏg��
	void AddRensa(void);												// �A���𑝂₷
	void ojamaCnt(int cnt);												// ���ז��v���𑊎�ɑ���t����Ƃ��Ɏg��
	void gameOverCnt(int cnt);											
	void victory(Victory vic);
	// ------ Get
	const int GetStageID(void)const;									// �X�e�[�WID�������ݗp
	const Vector2 offSet(void)const;									// �X�e�[�W�̃I�t�Z�b�g
	const Vector2 size(void) const;										// �T�C�Y�̎擾
	const int rensa(void)const;											// �A���̎擾
	const Vector2 GetGrid(Vector2 pos);									// �w�肵�����W�̃}�X�ڂ��擾;]
	const int ojamaCnt(void)const;										// �����̂��ז����Ď擾
	const int id(void)const;											// ���ā[����ID�擾
	const bool alive(void)const;										// �����Ă邩����ł邩
	const bool nextScene(void)const;
	const Victory& victory(void)const;
private:

	void Init(void);
	bool CheckMove(SharePuyo& vec);										// �㉺���E�����Ă�����
	void InstancePuyo();												// �V�����v��
	void InstanceOjama(int no);											// ���ז��v���̐���
	Vector2 offSet_;													// �X�e�[�W��ʂ̃I�t�Z�b�g
	Vector2 size_;
	int blockSize_;
	int stageID_;														// �X�e�[�W�̕`��p�X�N���[��ID
	int puyoID_;														// �Ղ�̕`��ID
	int guideID_;														// �K�C�h�p�X�N���[��
	int hideID_;														// �����܂���̂Ƃ�����B�����߂̃}�X�N���쐬
	int ojamaPuyoID_;													// ���ז��v���`��p
	Vector2 gameOverPos_;												// �Q�[���I�[�o�[�̎��ɓ������p
	double angle_;														// �X�e�[�W�̊p�x

	int maxRensa_;														// ���܂ł̍ő�A��
	int rensa_;															// �����A��
	int ojamaCnt_;														// �����ז�����́[
	int eraseCnt_;														// ���񉽌Ղ悫�����́[
	int gameOverCnt_;													// �Q�[���I�[�o�[�ɂȂ���

	bool ojamaFlg = false;

	std::unique_ptr<PlayUnit> playUnit_;								// ��ڲ԰�Ɋւ��鏈�����t�����h�ł�����ĊǗ�
	std::unique_ptr<NextPuyo> nextPuyo_;								// �l�N�X�g�v���Ǘ��p

	std::vector<SharePuyo> dataBase_;									// �X�e�[�W�S�̂̃}�X�Ǘ��p
	std::vector<SharePuyo*> data_;										// dataBase�̃|�C���^�[������

	std::vector<SharePuyo> eraseDataBase_;								// �X�e�[�W�S�̂̃}�X�łS�������������
	std::vector<SharePuyo*> erasedata_;									// eraseDataBase�̃|�C���^�[������

	std::map<INPUT_ID, bool> moveFlg_;									// �ړ����Ă������Btrue : ���b�N

	std::map<CntType,std::shared_ptr<InputState>> input_;				// �L�[�̓��͊Ǘ�

	int inputNum = 0;													// �y�[�W�A�b�v�y�[�W�_�E���ő���Ώۂ�؂�ւ�����

	std::vector<SharePuyo> puyoVec_;									// �Ղ�̃��X�g
	std::list<SharePuyo> ojamaList_;									// ���ז����X�g

	std::map<StgMode, std::function<void(Stage&)>> modeMap_;			// ���[�h�ʊ֐��I�u�W�F�N�g
	std::map<Victory, std::function<bool(Stage&)>> victoryMap_;			// ���������ŏ�����ς���

	static std::map<int, int> stgBG_;									// �X�e�[�W�ɂ���Ĕw�i��ς����悤��

	bool alive_;														// �X�e�[�W�������Ă邩

	static int playCnt_;												// �����l�����ꍇ�l���ŃL�[��ς����悤��
	int id_;															// �X�e�[�W�̔ԍ�
	StgMode stgMode_;
	Victory victory_;													// �����Ȃ̂������Ȃ̂����������Ȃ̂�

	bool nextScene_;													// ���̃V�[���Ɉړ����Ă������ǂ���

	friend class  PlayUnit;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct DropMode;
	friend struct MunyonMode;
	friend struct GameOver;
	friend struct DeleteMode;
	friend struct Draw;
	friend struct Lose;
	friend struct Win;

	//friend struct OjamaMode;
};

