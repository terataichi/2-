#include "MenuScene.h"
#include <DxLib.h>
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../Input/KeyState.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"
#include "GameEnd.h"

///
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

	// trueでアップデートを回す
	if (childUpdateFlg_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}

	for (auto key : input_)
	{

		if (buttonVec_[buttonCnt_].first->CheckHitButton(pos_))
		{

			if (key.second.first->GetTrgPull(INPUT_ID::BUTTON_MOVEON))
			{
				return std::move(nextMap_[buttonVec_[buttonCnt_].second]());
			}
			// 対象の動きを変える		
			buttonVec_[buttonCnt_].first->movePattern(MovePattern::UpDown);
		}

		key.second.second();

		// 操作処理のアップデート
		(*key.second.first)();
	}

	// buttonのアップデート
	buttonVec_[buttonCnt_].first->Update();

	return std::move(own);
}

void MenuScene::Draw(void)
{
	// trueで子の描画
	if (childDrawFlg_)
	{
		childScene_->Draw();
	}

	// ぼたんの描画
	for (auto& button : buttonVec_)
	{
		button.first->Draw();
	}
}

void MenuScene::Init(bool update, bool draw)
{
	childUpdateFlg_ = update;
	childDrawFlg_ = draw;
	pos_ = buttonVec_[0].first->stdPos();
	buttonCnt_ = 0;
	oldButtonCnt_ = buttonCnt_;
	InitFunc();
	for (auto key : input_)
	{
		key.second.first->SetUp(0);
	}
}

void MenuScene::InitFunc(void)
{
	// ----- インプット関係
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

	auto PadKeyUpdate = [&]()
	{
		oldButtonCnt_ = buttonCnt_;
		for (auto key : input_)
		{
			if (key.first != CntType::Mouse)
			{
				if (key.second.first->GetTrgPush(INPUT_ID::BUTTON_DOWN))
				{
					buttonCnt_++;
					CheckbuttonCnt(buttonCnt_, static_cast<int>(buttonVec_.size()));
					pos_ = buttonVec_[buttonCnt_].first->stdPos();
					buttonVec_[oldButtonCnt_].first->reset();
				}
				else if (key.second.first->GetTrgPush(INPUT_ID::BUTTON_UP))
				{
					buttonCnt_--;
					CheckbuttonCnt(buttonCnt_, static_cast<int>(buttonVec_.size()));
					pos_ = buttonVec_[buttonCnt_].first->stdPos();
					buttonVec_[oldButtonCnt_].first->reset();
				}
			}
		}
	};

	input_ =
	{
		{CntType::Pad, {std::make_shared<PadState>(),PadKeyUpdate}},
		// {CntType::Mouse, std::make_shared<MouseState>()},
		{CntType::Key, {std::make_shared<KeyState>(),PadKeyUpdate}}
	};


	// ----- シーン関係
	// 直接書くとダメみたいなので一度ﾗﾑﾀﾞにして使うときにメイクユニークするようにする
	auto game = []() {return std::make_unique<GameScene>(); };
	auto title = []() {return std::make_unique<TitleScene>(); };
	auto gameOver = []() {return std::make_unique<GameOverScene>(); };
	auto gameEnd = []() {return std::make_unique<GameEnd>(); };

	nextMap_.try_emplace(NextScene::Game, game);
	nextMap_.try_emplace(NextScene::Title, title);
	nextMap_.try_emplace(NextScene::GameOver, gameOver);
	nextMap_.try_emplace(NextScene::GameEnd, gameEnd);

}
