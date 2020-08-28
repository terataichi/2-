#include "MenuScene.h"
#include <DxLib.h>
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../Input/KeyState.h"


/// <summary>
/// 
/// </summary>
/// <param name="child"></param>
/// <param name="move"></param>
/// <param name="buttonNum"></param>
/// <param name="width"></param>
/// <returns></returns>
MenuScene::MenuScene(uniqueBase child, bool update, bool draw, ButtonPairVec buttonVec) :childScene_(std::move(child)), buttonVec_(std::move(buttonVec))
{
	Init(update, draw);
}

MenuScene::~MenuScene()
{
}

uniqueBase MenuScene::Update(uniqueBase own)
{
	//// 元のシーンに戻るよ
	//if (CheckHitKey(KEY_INPUT_F1))
	//{
	//	return std::move(childScene_);
	//}

	// trueでアップデートを回す
	if (childUpdateFlg_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}

	for (auto key : input_)
	{
		// 操作処理のアップデート
		(*key.second)();
	}

	for (auto& button : buttonVec_)
	{
		if (button.first->CheckHitButton(pos_))
		{
			// 選択されてるから決定ボタンが押されるとそのbuttonが持っているシーンに行ける
			for (auto& key : input_)
			{
				if (key.second->GetTrgPull(INPUT_ID::BUTTON_UP))
				{
					return std::move(button.second);
				}
			}

			// 対象の動きを変える
			button.first->movePattern(MovePattern::UpDown);
		}

		// buttonのアップデート
		button.first->Update();
	}

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
	pos_ = buttonVec_[0].first->pos();

	input_ =
	{
		// {CntType::Pad, std::make_shared<PadState>()},
		// {CntType::Mouse, std::make_shared<MouseState>()},
		{CntType::Key, std::make_shared<KeyState>()}
	};

	for (auto key : input_)
	{
		key.second->SetUp(0);
	}
}
