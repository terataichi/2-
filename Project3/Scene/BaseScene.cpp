#include "BaseScene.h"
#include <DxLib.h>
#include "SceneMng.h"

int BaseScene::count_ = 0;

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