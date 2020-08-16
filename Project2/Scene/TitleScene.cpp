#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"

TitleScene::TitleScene()
{
	newKey_ = 0;
	oldKey_ = 0;
}

TitleScene::~TitleScene()
{
}

uniqueBase TitleScene::Update(uniqueBase own)
{
	oldKey_ = newKey_;
	newKey_ = CheckHitKey(KEY_INPUT_SPACE);
	if (!newKey_ && oldKey_)
	{
		return std::make_unique<GameScene>();
	}

	return std::move(own);
}

void TitleScene::Draw(void)
{
	DrawBox(0, 0, lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, 0xffffff, true);

	SetFontSize(64);
	DrawFormatString(lpSceneMng.screenSize_.x / 3 + 64, lpSceneMng.screenSize_.y / 2, 0xf, "ƒ^ƒCƒgƒ‹");
	DrawFormatString(lpSceneMng.screenSize_.x / 3 - 32, lpSceneMng.screenSize_.y - lpSceneMng.screenSize_.y / 3, 0xf, "Please Hit Space");
}
