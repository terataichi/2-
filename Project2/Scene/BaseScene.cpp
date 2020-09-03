#include "BaseScene.h"

BaseScene::BaseScene()
{
	nextScene_ = Scene::Non;
	menuFlg_ = false;
	scene_ = Scene::Base;
}

BaseScene::~BaseScene()
{
}

void BaseScene::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}

bool BaseScene::nextUpdate(void)
{
	return true;
}

void BaseScene::SetNextScene(Scene next)
{
	nextScene_ = next;
}

Scene BaseScene::GetNextScene(void)
{
	return nextScene_;
}
