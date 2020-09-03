#pragma once
#include <vector>
#include "BaseScene.h"
#include "../common/Button.h"
#include "../Input/InputState.h"

using uniqueButton = std::unique_ptr<Button>;

// ���̃V�[���𐶐����邽�߂̃}�b�v
using NextMap = std::pair < Scene, std::function<uniqueBase(void)> >;

// �ڂ���ɑΉ������V�[�����i�[����
using ButtonPairVec = std::vector<std::pair<uniqueButton, NextMap>>;

// �e������@�ɉ����č��W�̑ւ������Ⴄ
using InputPosP = std::pair<std::shared_ptr<InputState>, std::function<void(std::shared_ptr<InputState>&)>>;

class MenuScene :
	public BaseScene
{
public:
	MenuScene(uniqueBase child, bool update, bool draw, ButtonPairVec buttonVec);
	~MenuScene();

	uniqueBase Update(uniqueBase own) override;
	void Draw(void)override;
	Scene scene()override;
	void SetMenuFlg(bool set)override;
private:
	void Init(bool update, bool draw);
	void InitFunc(void);

	bool childUpdateFlg_;												// ���j���[�Ŏq�̃A�b�v�f�[�g�𓮂������������Ȃ���
	bool childDrawFlg_;													// ���j���[�Ŏq�̕`������邩�ǂ���
	uniqueBase childScene_;												// ���j���[���J�����Ƃ��ɑO�̃V�[����ۑ����Ă���
	bool menuFlg_;
	ButtonPairVec buttonVec_;											// �ڂ���Ǘ��p
	Scene scene_;
	bool nextFlg_;														// ���̃V�[���ɍs���Ă������ǂ���

	std::map<CntType, InputPosP> input_;								// �L�[�̓��͊Ǘ�
	int buttonCnt_;														// �ڂ���̔ԍ�
	int oldButtonCnt_;													// �O�̃{�^���̔ԍ�

	Vector2 pos_;														// �ڂ���Ƃ̓����蔻��p
};

