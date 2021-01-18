#include "StarScene.h"
#include "../Scene/SceneMng.h"
#include <DxLib.h>

StarScene::StarScene(uniqueBase prev, uniqueBase next)
{
	prevScene_ = std::move(prev);
	nextScene_ = std::move(next);
	Init();
}

StarScene::~StarScene()
{
}

uniqueBase StarScene::Update(uniqueBase scene)
{
	DrawOwnScene();

	bool next = true;
	for (auto& list : starList_)
	{
		list->Update();
		next &= list->pos().x - lpSceneMng.screenSize_.x * 2 > lpSceneMng.screenSize_.x;
	}

	// ボックスの一番後ろが画面からいなくなったら次のシーンへ
	if (next)
	{
		TRACE("次のシーンに移動します\n");
		return std::move(nextScene_);
	}

	return scene;
}

void StarScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();

	if (nextFlg_)
	{
		nextScene_->Draw();

		for (auto& list : starList_)
		{
			DrawBox(list->pos().x - lpSceneMng.screenSize_.x * 2, list->pos().y - 44, list->pos().x, list->pos().y + 43, list->colorCode(), true);
			list->Draw();
		}
	}
	else
	{
		prevScene_->Draw();
		bool flg = true;
		for (auto& list : starList_)
		{
			DrawBox(0, list->pos().y - 44, list->pos().x, list->pos().y + 43, list->colorCode(), true);
			list->Draw();

			flg &= list->pos().x + 46 >= lpSceneMng.screenSize_.x;
		}
		nextFlg_ = flg;
	}
}

void StarScene::Init(void)
{
	for (int i = 0; i < 7; i++)
	{
		auto pos = Vector2{ -91 * i,44 + 87 * i };
		starList_.emplace_back(std::make_shared<Star>(pos, Vector2{ 5,0 }, i));
	}
}
