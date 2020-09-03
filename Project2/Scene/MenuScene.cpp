#include "MenuScene.h"
#include <DxLib.h>
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../Input/KeyState.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameEnd.h"

MenuScene::MenuScene(uniqueBase child, bool update, bool draw, ButtonPairVec buttonVec):
childScene_(std::move(child)), buttonVec_(std::move(buttonVec))
{
	Init(update, draw);
}

MenuScene::~MenuScene()
{
}

uniqueBase MenuScene::Update(uniqueBase own)
{
	// true�ŃA�b�v�f�[�g����
	if (childUpdateFlg_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}



	// �{�^����������Ă��Ă����̃V�[���ł�肽�����Ƃ��I����Ă����玟�̃V�[����
	if (nextFlg_)
	{
		// ���̃V�[���ɍs���Ă������ǂ���
		if (childScene_->nextUpdate())
		{
			if (childScene_->scene() != buttonVec_[buttonCnt_].second.first)
			{
				return std::move(buttonVec_[buttonCnt_].second.second());
			}
			else
			{
				// �����V�[���������猳�ɖ߂�
				childScene_->SetMenuFlg(false);
				return std::move(childScene_);
			}
		}
	}
	else
	{
		for (auto& key : input_)
		{
			// �����蔻��
			if (buttonVec_[buttonCnt_].first->CheckHitButton(pos_))
			{
				// ������Ă��邩
				nextFlg_ |= key.second.first->GetTrgPush(INPUT_ID::BUTTON_MOVEON);
				childScene_->SetNextScene(buttonVec_[buttonCnt_].second.first);
				// �Ώۂ̓�����ς���
				buttonVec_[buttonCnt_].first->movePattern(MovePattern::UpDown);
			}
			// �{�^���̐؂�ւ�������
			key.second.second(key.second.first);

			// ���쏈���̃A�b�v�f�[�g
			(*key.second.first)();
		}
	}

	// �{�^���̃A�b�v�f�[�g
	buttonVec_[buttonCnt_].first->Update();

	return std::move(own);
}

void MenuScene::Draw(void)
{
	// true�Ŏq�̕`��
	if (childDrawFlg_)
	{
		childScene_->Draw();
	}

	// �ڂ���̕`��
	for (auto& button : buttonVec_)
	{
		button.first->Draw();
	}
}

Scene MenuScene::scene()
{
	return scene_;
}

void MenuScene::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}

void MenuScene::Init(bool update, bool draw)
{
	childUpdateFlg_ = update;
	childDrawFlg_ = draw;
	pos_ = buttonVec_[0].first->stdPos();
	buttonCnt_ = 0;
	oldButtonCnt_ = buttonCnt_;
	scene_ = Scene::Menu;
	menuFlg_ = false;
	nextFlg_ = false;
	InitFunc();
	for (auto key : input_)
	{
		key.second.first->SetUp(0);
	}
}

void MenuScene::InitFunc(void)
{
	// ----- �C���v�b�g�֌W
	auto CheckbuttonCnt = [](int& cnt, int max)
	{
		if (cnt >= max)
		{
			cnt = max - 1;
		}
		if (cnt <= 0)
		{
			cnt = 0;
		}
	};

	auto PadKeyUpdate = [&](std::shared_ptr<InputState>& type)
	{
		oldButtonCnt_ = buttonCnt_;

		if (type->GetCntType() != CntType::Mouse)
		{
			if (type->GetTrgPush(INPUT_ID::BUTTON_DOWN))
			{
				buttonCnt_++;
				CheckbuttonCnt(buttonCnt_, static_cast<int>(buttonVec_.size()));
				pos_ = buttonVec_[buttonCnt_].first->stdPos();
				buttonVec_[oldButtonCnt_].first->reset();
			}
			else if (type->GetTrgPush(INPUT_ID::BUTTON_UP))
			{
				buttonCnt_--;
				CheckbuttonCnt(buttonCnt_, static_cast<int>(buttonVec_.size()));
				pos_ = buttonVec_[buttonCnt_].first->stdPos();
				buttonVec_[oldButtonCnt_].first->reset();
			}
		}
	};

	input_ = {
		{CntType::Pad, {std::make_shared<PadState>(),PadKeyUpdate}},
		// {CntType::Mouse, std::make_shared<MouseState>()},
		{CntType::Key, {std::make_shared<KeyState>(),PadKeyUpdate}}
	};
}
