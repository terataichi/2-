#include <algorithm>
#include <DxLib.h>
#include "Stage.h"
#include "SceneMng.h"

Stage::Stage()
{
	_stageID = MakeScreen(lpSceneMng._gameSize.x, lpSceneMng._gameSize.y);
}

Stage::Stage(Vector2&& offSet,Vector2&& size) :_offSet(std::move(offSet)), _size(std::move(size))
{
	Init();
}

Stage::~Stage()
{
}

const int Stage::GetStageID(void) const
{
	return Stage::_stageID;
}

const Vector2 Stage::offSet(void) const
{
	return Stage::_offSet;
}

const Vector2 Stage::size(void) const
{
	return Stage::_size;
}

void Stage::Draw(void)
{
	SetDrawScreen(_stageID);
	ClsDrawScreen();
	DrawBox(0,0,lpSceneMng._gameSize.x,lpSceneMng._gameSize.y, 0xfff, true);
}

void Stage::UpDate(void)
{
}

void Stage::Init()
{
	_stageID = MakeScreen(_size.x, _size.y);
}
