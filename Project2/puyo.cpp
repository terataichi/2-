#include <DxLib.h>
#include "puyo.h"
#include "SceneMng.h"

puyo::puyo() :_size(64, 64), _rad(_size / 2)
{
	Init();
}

puyo::~puyo()
{
}

void puyo::UpDate(void)
{

	_pos.y += 2;

	if (lpSceneMng._gameSize.y - _rad.y > _pos.y)
	{
	}
}

void puyo::Move(INPUT_ID id)
{
	//// âÊñ äOÇ…çsÇ¡ÇΩèÍçáÇÃèCê≥
	auto OutPos = [&](DirUnion check, Vector2&& fix)
	{
		if (check)
		{
			_pos = fix;
		}
	};
	_pos += _vec[id];
	OutPos(_dirFlg, Vector2(_rad.x, _pos.y));
	OutPos(_pos.y < 0, Vector2(_pos.x, _rad.y));
	OutPos(_pos.x > lpSceneMng._gameSize.x, Vector2(lpSceneMng._gameSize.x - _rad.x, _pos.y));
	OutPos(_pos.y > lpSceneMng._gameSize.y - _size.y, Vector2(_pos.x, lpSceneMng._gameSize.y - _rad.y));
}

void puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, lpSceneMng._pyoRadius, 0xfffff, true);
}

bool puyo::SetDirFlg(DirUnion flg)
{
	_dirFlg = flg;
	return true;
}

const Vector2 puyo::pos() const
{
	return _pos;
}

const Vector2 puyo::size(void) const
{
	return _size;
}

const Vector2 puyo::rad(void) const
{
	return _rad;
}

void puyo::Init(void)
{
	_pos = Vector2(32, 32);
	_vec =
	{
		{INPUT_ID::BUTTON_LEFT,Vector2(-_size.x, 0)},
		{INPUT_ID::BUTTON_UP,Vector2(0, -_size.y)},
		{INPUT_ID::BUTTON_RIGHT,Vector2(_size.x, 0)},
		{INPUT_ID::BUTTON_DOWN,Vector2(0,_size.y)},
		{INPUT_ID::BUTTON_ROTA,Vector2(0,0)},
	};
}
