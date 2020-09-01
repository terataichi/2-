#include "GameOverScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "MenuScene.h"
#include "../common/ImageMng.h"


GameOverScene::GameOverScene()
{
	scene_ = Scene::GameOver;
	menuFlg_ = false;
}

GameOverScene::~GameOverScene()
{
}

uniqueBase GameOverScene::Update(uniqueBase own)
{
	// ÉÅÉjÉÖÅ[ÇäJÇ≠
	if (!menuFlg_)
	{
		ButtonPairVec button;
		int cnt = 0;
	
		Vector2 tmpPos = { lpSceneMng.screenSize_.x / 2,lpSceneMng.screenSize_.y / 3 };
		Vector2 tmpSize{ 320,64 };

		button.emplace_back(std::make_unique<Button>("Continue", tmpPos, tmpSize, 1, 0.0f, -100, cnt), Scene::Game);
		++cnt;
		tmpPos.y += 200;
		button.emplace_back(std::make_unique<Button>("GoTitle", tmpPos, tmpSize, 1, 0.0f, -100, cnt), Scene::Title);
		++cnt;
		tmpPos.y += 200;
		button.emplace_back(std::make_unique<Button>("EndGame", tmpPos, tmpSize, 1, 0.0f, -100, cnt), Scene::GameEnd);
		menuFlg_ = true;
		return std::make_unique<MenuScene>(std::move(own), true, true, std::move(button));
	}

	return std::move(own);
}

void GameOverScene::Draw(void)
{
	Vector2 tmp{ lpSceneMng.screenSize_ / 2 };
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0],tmp.x,tmp.y,1 , 0.0f, 0 });
}

Scene GameOverScene::scene()
{
	return scene_;
}

void GameOverScene::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}
