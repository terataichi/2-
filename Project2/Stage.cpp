#include <DxLib.h>
#include "Stage.h"
#include "SceneMng.h"

Stage::Stage()
{
	_StageID = MakeScreen(lpSceneMng._gameSize.x, lpSceneMng._gameSize.y);
}

Stage::Stage(Vector2 pos)
{
	Init(pos);
}

Stage::~Stage()
{
}

const int Stage::GetStageID(void) const
{
	return _StageID;
}

void Stage::Draw(void)
{
	SetDrawScreen(_StageID);
	ClsDrawScreen();
	DrawBox(0,0, _pos.x + lpSceneMng._gameSize.x, _pos.y + lpSceneMng._gameSize.y, 0xfff, true);
}

void Stage::Init(Vector2 pos)
{
	_StageID = MakeScreen(lpSceneMng._gameSize.x, lpSceneMng._gameSize.y);
	_pos = pos;
}
