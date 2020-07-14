#pragma once
#include <memory>
#include <vector>
#include <map>
#include "common/Vector2.h"
#include "Input/InputState.h"
#include "puyo.h"

#define STAGE_X 6
#define STAGE_Y 13

class Stage
{
public:
	Stage(Vector2&& offSet, Vector2&& size);
	~Stage();

	const int GetStageID(void)const;					// �X�e�[�WID�������ݗp
	const Vector2 offSet(void)const;
	const Vector2 size(void) const;
	void Draw(void);									// �e�X�N���[���ɕ`�悷��
	void UpDate(void);									// �X�V.
private:
	void Init();
	Vector2 _offSet;									// �X�e�[�W��ʂ̃I�t�Z�b�g
	Vector2 _size;
	int _stageID;										// �X�e�[�W�̕`��p�X�N���[��ID

	std::vector<int> _dataBase;							// �X�e�[�W�S�̂̃}�X�Ǘ��p
	std::vector<int*> _data;							// dataBase�̃|�C���^�[������

	std::map<INPUT_ID, bool> _moveFlg;					// �ړ����Ă������Btrue : ���b�N

	std::shared_ptr<InputState> _input;					// �L�[�̓��͊Ǘ�

	std::shared_ptr<puyo> _puyo;

	static int playCnt;									// �����l�����ꍇ�l���ŷ���ς����悤��
	int _id;
};

