#include <DxLib.h>
#include "puyo.h"
#include "SceneMng.h"
#include "INPUT_ID.h"

puyo::puyo()
{
	Init();
}

puyo::~puyo()
{
}

void puyo::UpDate(void)
{
	_pos.y += 2;
}

void puyo::Move(const Vector2& vec)
{
	_pos += vec;
}

void puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, lpSceneMng._pyoRadius, 0xfffff, true);
}

void puyo::Init(void)
{
	_pos = Vector2(32, 32);
}
