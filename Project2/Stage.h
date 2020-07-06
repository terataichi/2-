#pragma once
#include "common/Vector2.h"

class Stage
{
public:
	Stage();
	Stage(Vector2 pos);
	~Stage();

	const int GetStageID(void)const;					// �X�e�[�WID�������ݗp
	void Draw(void);									// �e�X�N���[���ɕ`�悷��
private:
	void Init(Vector2 pos);

	Vector2 _pos;
	int _StageID;										// �X�e�[�W�̕`��p�X�N���[��ID
};

