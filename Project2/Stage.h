#pragma once
#include <memory>
#include "common/Vector2.h"
#include "InputState.h"
#include "puyo.h"

class Stage
{
public:
	Stage();
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

	std::shared_ptr<InputState> _input;					// �L�[�̓��͊Ǘ�

	std::shared_ptr<puyo> _puyo;
};

