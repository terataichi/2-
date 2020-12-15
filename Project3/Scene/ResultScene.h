#pragma once
#include "BaseScene.h"
#include "../NetWork/NetWork.h"

class ResultScene :
    public BaseScene
{
public:

	/// <summary>
	/// �C���X�^���X���Ƀ��U���g�̃f�[�^��n��
	/// </summary>
	/// <param name="resultData"></param>
	ResultScene(UnionVec& resultData);

	~ResultScene();

	uniqueBase Update(uniqueBase scene)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:
	UnionVec resultData_;
};

