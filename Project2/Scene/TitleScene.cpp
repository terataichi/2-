#include <DxLib.h>
#include "SceneMng.h"
#include "../common/ImageMng.h"
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
	Vector2 tmp{ lpSceneMng.screenSize_ / 2 };
	lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("BG")[0],tmp.x,tmp.y,1 , 0.0f, 0 });
}
