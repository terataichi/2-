#pragma once
#include "../../Stage.h"

struct Win
{
	bool operator()(Stage& stage)
	{
		//TRACE("プレイヤー %d Win\n",stage.id_ + 1);
		if (stage.gameOverCnt_ < 0)
		{
			return true;
		}
		stage.gameOverCnt_--;

		int standard = 0;
		float angle = 0;
		float angleStnd = 36.0f;

		Vector2 div{ 5,1 };
		for (int j = 0; j < div.x; j++)
		{
			int id = lpImageMng.GetHandle("win", div, { 64,64 })[j];


			standard = static_cast<int>((stage.size_.y / 3) - 50.0f * cosf((count + (j + 1) * 30.0f) * 0.01f));			// サインカーブ

			angle = (-DX_PI_F / angleStnd) - (DX_PI_F / (angleStnd / 2)) * cosf((count + (j + 1) * 30.0f) * 0.01f);

			lpSceneMng.AddDrawQue({ id, {stage.offSet_.x + 64 + 64 * j, standard}, angle, 0 });
			count++;
		}
		return false;
	}

private:
	int speed = 10;
	int count = 0;

};