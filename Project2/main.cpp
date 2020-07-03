#include <DxLib.h>
#include "common/Vector2.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	
	Vector2 _pos = Vector2(0,0);
	Vector2 _offSet = Vector2(66, 66);
	int _pyoSize = 64;
	int _pyoR = _pyoSize / 2;
	Vector2 _gameSize = Vector2(_pyoSize * 6, _pyoSize * 13);
	Vector2 _frameSize = Vector2(64, 66);
	Vector2 _screenSize = Vector2(_gameSize.x * 3 + _frameSize.x * 2, _gameSize.y + _offSet.y);

	ChangeWindowMode(true);
	SetWindowText("1916025_éõçËëÂíq");
	SetGraphMode(_screenSize.x, _screenSize.y, 16);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		//if (CheckHitKey(KEY_INPUT_D))
		//{
		//	_pos.x += _pyoSize;
		//}

		//_pos.y += _pyoSize / (_pyoSize / 3);

		DrawBox(_frameSize.x, _frameSize.y, _frameSize.x + _pyoSize * 6, _pyoSize *13, 0xfff, true);
		DrawBox(_frameSize.x + _gameSize.x * 2, _offSet.y, _frameSize.x + _gameSize.x * 2 + _pyoSize * 6, _pyoSize * 13, 0xfff, true);

		for (int j = 0; j < 13; j++)
		{
			DrawCircle(_frameSize.x + _pyoR + _pos.x, _frameSize.y + _pyoR + _pos.y + (_pyoSize * j), _pyoR, 0xfffff, true);
		}
		ScreenFlip();
	}
}