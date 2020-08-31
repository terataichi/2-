#include <DxLib.h>
#include <cmath>
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"

TitleScene::TitleScene()
{
	Init();
}

TitleScene::~TitleScene()
{
}

uniqueBase TitleScene::Update(uniqueBase own)
{
	// メニューを開く
	if (!MenuFlg_)
	{
		ButtonPairVec button;
		int cnt = 0;
		Vector2 tmpPos = { lpSceneMng.screenSize_.x / 2, lpSceneMng.screenSize_.y / 2 + 50};
		Vector2 tmpSize{ 320,64 };

		button.emplace_back(std::make_unique<Button>("StartGame", tmpPos, tmpSize, 1, 0.0f, -100, cnt), NextScene::Game);
		++cnt;
		tmpPos.y += 200;
		button.emplace_back(std::make_unique<Button>("EndGame", tmpPos, tmpSize, 1, 0.0f, -100, cnt), NextScene::GameEnd);

		MenuFlg_ = true;
		return std::make_unique<MenuScene>(std::move(own), true, true,std::move(button));
	}

	count_++;
	return std::move(own);
}

void TitleScene::Draw(void)
{
	Vector2 tmp{ lpSceneMng.screenSize_ / 2 };
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0],tmp.x,tmp.y,1 , 0.0f, 0});


	// 文字ごとに動きを付ける
	Vector2 div{ 5,1 };
	Vector2 tmpPos{ 0 ,0 };
	double tmpRate = 0;
	for (int j = 0; j < div.x; j++)
	{
		int id = lpImageMng.GetHandle("TitleName", div, { 144,124 })[j];

		//tmpPos.y = static_cast<int>(logoPos_.y - 50.0f * cosf((count_ + (j + 1) * 30.0f) * 0.015f));			// サインカーブ
		tmpRate = static_cast<double>(1 + 0.1 * sin((static_cast<double>(count_ + (j + 1.0) * 20) * 0.05)));

		lpSceneMng.AddDrawQue({ id,logoPos_.x + (144 * j),logoPos_.y, tmpRate, 0.0f, 0 });
	}
}

bool TitleScene::Init(void)
{
	MenuFlg_ = false;
	count_ = 0;
	logoPos_ = { lpSceneMng.screenSize_.x / 4 + 38 ,lpSceneMng.screenSize_.y / 4 };
	return false;
}
