#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"
#include "../Input/InputState.h"

using uniqueButton = std::unique_ptr<Button>;

// �ڂ���ɑΉ������V�[�����i�[����
using ButtonPairVec = std::vector<std::pair<uniqueButton, uniqueBase>>;

class MenuScene :
	public BaseScene
{
public:
	MenuScene(uniqueBase child, bool update, bool draw, ButtonPairVec buttonVec);
	~MenuScene();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
private:
	void Init(bool update, bool draw);

	bool childUpdateFlg_;																// ���j���[�Ŏq�̃A�b�v�f�[�g�𓮂������������Ȃ���
	bool childDrawFlg_;																	// ���j���[�Ŏq�̕`������邩�ǂ���
	uniqueBase childScene_;																// ���j���[���J�����Ƃ��ɑO�̃V�[����ۑ����Ă���
	ButtonPairVec buttonVec_;															// �ڂ���Ǘ��p
	std::map<CntType, std::shared_ptr<InputState>> input_;								// �L�[�̓��͊Ǘ�

	
	Vector2 pos_;																		// �ڂ���Ƃ̓����蔻��p
};

