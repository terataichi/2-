#include "GameEnd.h"

GameEnd::GameEnd()
{
	scene_ = Scene::GameEnd;
	menuFlg_ = false;
}

GameEnd::~GameEnd()
{
}

uniqueBase GameEnd::Update(uniqueBase own)
{
	lpSceneMng.SetEnd();
	return own;
}

void GameEnd::Draw(void)
{
}

Scene GameEnd::scene()
{
	return scene_;
}

void GameEnd::SetMenuFlg(bool set)
{
	menuFlg_ = set;
}
