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
	switch (id)
	{
	case INPUT_ID::BUTTON_LEFT:
		if (!_dirFlg.bit.left) { _pos.x -= _size.x; }
		break;
	case INPUT_ID::BUTTON_UP:
		if (!_dirFlg.bit.up) { _pos.y -= _size.x; }
		break;
	case INPUT_ID::BUTTON_RIGHT:
		if (!_dirFlg.bit.right) { _pos.x += _size.x; }
		break;
	case INPUT_ID::BUTTON_DOWN:
		if (!_dirFlg.bit.down) { _pos.y += _size.y; }
		break;
	case INPUT_ID::BUTTON_ROTA:
		break;
	case INPUT_ID::MAX:
		break;
	default:
		break;
	}

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
}
