#include <DxLib.h>
#include "puyo.h"
#include "SceneMng.h"
#include "INPUT_ID.h"

puyo::puyo() :_size(64, 64), _rad(_size / 2)
{
	Init();
}

puyo::~puyo()
{
}

void puyo::UpDate(void)
{
	//_pos.y += 2;
}

void puyo::Move(const Vector2& vec)
{
	_pos += vec;

	if (_pos.x + vec.x < 0)
	{
		_pos.x = _rad.x;
	}
	if (_pos.y + vec.y < 0)
	{
		_pos.y = _rad.y;
	}
	if (_pos.x + vec.x > lpSceneMng._gameSize.x)
	{
		_pos.x = lpSceneMng._gameSize.x - _rad.x;
	}
	if (_pos.y + vec.y > lpSceneMng._gameSize.y)
	{
		_pos.y = lpSceneMng._gameSize.y - _rad.y;
	}
}

void puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, lpSceneMng._pyoRadius, 0xfffff, true);
}

bool puyo::IsCheckPos(void)
{

	return false;
}

void puyo::Init(void)
{
	_pos = Vector2(32, 32);
}
