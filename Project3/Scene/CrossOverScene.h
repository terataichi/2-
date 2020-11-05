#pragma once
#include "TransitionScene.h"

class CrossOverScene:
	public TransitionScene
{
public:
	/// <summary>
	/// �����ɑO�̃V�[���Ǝ��̃V�[����n��
	/// </summary>
	/// <param name="prev"></param>
	/// <param name="next"></param>
	/// <returns></returns>
	CrossOverScene(uniqueBase prev, uniqueBase next);

	~CrossOverScene();

	uniqueBase Update(uniqueBase scene)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:

	int fadeCount_;
};

