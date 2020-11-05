#include "BaseScene.h"
#include <DxLib.h>
#include "SceneMng.h"

BaseScene::BaseScene()
{
	drawScreen_ = MakeScreen(lpSceneMng.screenSize_.x, lpSceneMng.screenSize_.y, true);
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw()
{
	DrawGraph(0, 0, drawScreen_, true);
}

void BaseScene::DrawOwnScene()
{
}