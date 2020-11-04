#include "CrossOverScene.h"
#include <DxLib.h>
#include "../_debug/_DebugConOut.h"

CrossOverScene::CrossOverScene(uniqueBase prev, uniqueBase next)
{
	prevScene_ = std::move(prev);
	nextScene_ = std::move(next);
	Init();
}

CrossOverScene::~CrossOverScene()
{
}

uniqueBase CrossOverScene::Update(uniqueBase scene)
{

	DrawOwnScene();

	if (++fadeCount_ >= 255)
	{
		TRACE("ŽŸ‚ÌƒV[ƒ“‚ÉˆÚ“®‚µ‚Ü‚·\n");
		return std::move(nextScene_);
	}
	return scene;
}

void CrossOverScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeCount_);
	nextScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - fadeCount_);
	prevScene_->Draw();


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void CrossOverScene::Init()
{
	fadeCount_ = 0;
}
