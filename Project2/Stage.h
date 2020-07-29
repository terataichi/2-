#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 8
#define STAGE_Y 15

class PlayUnit;

using SharePuyo = std::shared_ptr<puyo>;

enum class StgMode
{
	Drop,
	Puyon,
	Rensa,
	FALL
};

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;									// �X�e�[�WID�������ݗp
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	const Vector2 GetGrid(Vector2 pos);									// �w�肵�����W�̃}�X�ڂ��擾;
	bool SetErase(SharePuyo& puyo, Vector2 vec);						// �Ղ�������̂��Z�b�g
	void Draw(void);													// �e�X�N���[���ɕ`�悷��
	void UpDate(void);													// �X�V.
private:

	void Init(void);
	bool CheckMove(SharePuyo& vec);										// �㉺���E�����Ă�����
	void InstancePuyo();												// �V�����v��
	Vector2 offSet_;													// �X�e�[�W��ʂ̃I�t�Z�b�g
	Vector2 size_;
	int blockSize_;
	int stageID_;														// �X�e�[�W�̕`��p�X�N���[��ID

	std::unique_ptr<PlayUnit> playUnit_;								// ��ڲ԰�Ɋւ��鏈�����t�����h�ł�����ĊǗ�

	std::vector<SharePuyo> dataBase_;									// �X�e�[�W�S�̂̃}�X�Ǘ��p
	std::vector<SharePuyo*> data_;										// dataBase�̃|�C���^�[������

	std::vector<SharePuyo> eraseDataBase_;								// �X�e�[�W�S�̂̃}�X�łS�������������
	std::vector<SharePuyo*> erasedata_;									// eraseDataBase�̃|�C���^�[������

	std::map<INPUT_ID, bool> moveFlg_;									// �ړ����Ă������Btrue : ���b�N

	std::shared_ptr<InputState> input_;									// �L�[�̓��͊Ǘ�

	std::vector<SharePuyo> puyoVec_;

	std::map<StgMode, std::function<void(Stage&)>> modeMap_;			// ���[�h�ʊ֐��I�u�W�F�N�g

	static int playCnt_;												// �����l�����ꍇ�l���ŃL�[��ς����悤��
	int id_;
	StgMode stgMode_;

	friend class  PlayUnit;
	friend struct FallMode;
	friend struct RensaMode;
	friend struct PuyonMode;
	friend struct DropMode;

};

