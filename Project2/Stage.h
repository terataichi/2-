#pragma once
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 8
#define STAGE_Y 15

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;					// �X�e�[�WID�������ݗp
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	bool SetErase(void);								// �Ղ�������̂��Z�b�g
	void Draw(void);									// �e�X�N���[���ɕ`�悷��
	void UpDate(void);									// �X�V.
private:
	void Init();
	Vector2 offSet_;									// �X�e�[�W��ʂ̃I�t�Z�b�g
	Vector2 size_;
	int blockSize_;
	int stageID_;										// �X�e�[�W�̕`��p�X�N���[��ID

	std::vector<PuyoID> dataBase_;						// �X�e�[�W�S�̂̃}�X�Ǘ��p
	std::vector<PuyoID*> data_;							// dataBase�̃|�C���^�[������

	std::vector<PuyoID> eraseDataBase_;					// �X�e�[�W�S�̂̃}�X�Ǘ��p
	std::vector<PuyoID*> erasedata_;					// dataBase�̃|�C���^�[������

	std::map<INPUT_ID, bool> moveFlg_;					// �ړ����Ă������Btrue : ���b�N

	std::shared_ptr<InputState> input_;					// �L�[�̓��͊Ǘ�

	std::vector<std::shared_ptr<puyo>> puyoVec_;

	static int playCnt_;								// �����l�����ꍇ�l���ŷ���ς����悤��
	int id_;
};

