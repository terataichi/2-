#include "BaseScene.h"

BaseScene::BaseScene()
{
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
