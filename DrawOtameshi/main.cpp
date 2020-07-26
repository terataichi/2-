#include <math.h>
#include "Dxlib.h"
#include "common/Vector2.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);

	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	float count = 0;
	float time = 0;

	Vector2f pos = {300,300};
	Vector2f radius = { 64,64 };

	Vector2f standard = radius;
	Vector2f standardP = pos;

	while (ProcessMessage() == 0) {
		ClearDrawScreen();


		radius.y = standard.y - 10 * cosf(count * 0.02f);
		pos.y = standardP.y - 100.0f * cosf(count * 0.02f);

		DrawOval(pos.x, pos.y, radius.x, radius.y, 0xfff, true);

		count++;
		if ((int)count % 60 == 0)
		{
			time++;
		}
		ScreenFlip();
	}

	DxLib_End();


}