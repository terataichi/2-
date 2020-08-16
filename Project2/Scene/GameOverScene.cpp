#include "GameOverScene.h"
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
	oldKey_ = 0;
	newKey_ = 0;
}

GameOverScene::~GameOverScene()
{
}

uniqueBase GameOverScene::Update(uniqueBase own)
{
	oldKey_ = newKey_;
	newKey_ = CheckHitKey(KEY_INPUT_SPACE);
	if (!newKey_ && oldKey_)
	{
		return std::make_unique<TitleScene>();
	}

	return std::move(own);
}

void GameOverScene::Draw(void)
{
	DrawBox(0, 0, lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, 0xf, true);

	SetFontSize(64);
	DrawFormatString(lpSceneMng.screenSize_.x / 3 + 64, lpSceneMng.screenSize_.y / 2, 0xffffff, "ゲームオーバー");
	DrawFormatString(lpSceneMng.screenSize_.x / 3 - 32, lpSceneMng.screenSize_.y - lpSceneMng.screenSize_.y / 3, 0xff, "Please Hit Space");
}