#include <DxLib.h>
#include "SceneMng.h"

SceneMng* SceneMng::_sInstance;

void SceneMng::Run(void)
{
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		if (CheckHitKey(KEY_INPUT_D))
		{
			_pos.x += _pyoSize;
		}
		if (CheckHitKey(KEY_INPUT_A))
		{
			_pos.x -= _pyoSize;
		}
		_pos.y += _pyoSize / 32;

		Draw();
	}
}

void SceneMng::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();


	DrawBox(_frameSize.x, _frameSize.y, _frameSize.x + _pyoSize * 6, _pyoSize * 12, 0xfff, true);
	DrawBox(_frameSize.x + _gameSize.x * 2, _gameOffSet.y, _frameSize.x + _gameSize.x * 2 + _pyoSize * 6, _pyoSize * 12, 0xfff, true);


	DrawCircle(_frameSize.x + _pyoRadius + _pos.x, _frameSize.y + _pyoRadius + _pos.y + (_pyoSize), _pyoRadius, 0xfffff, true);

	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916025_éõçËëÂíq");
	SetGraphMode(_screenSize.x, _screenSize.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	return true;
}

void SceneMng::Init(void)
{
	_pos = Vector2f(0, 0);
}

SceneMng::SceneMng() :
	_pyoSize(64), _pyoRadius(_pyoSize / 2), _gameSize(Vector2(_pyoSize * 6, _pyoSize * 13)), _gameOffSet(Vector2(66, 66)),
	_frameSize(Vector2(64, 66)), _screenSize(Vector2(_gameSize.x * 3 + _frameSize.x * 2, _gameSize.y + _gameOffSet.y))
{
	SysInit();
	Init();
}

SceneMng::~SceneMng()
{
}
