#pragma once
#include "../../Stage.h"

struct Win
{
	bool operator()(Stage& stage)
	{
		// 一定の場所までステージを落とす
		int maxY = lpSceneMng.screenSize_.y + stage.size_.y;
		if (winY < maxY)
		{
			winY += speed;
		}

		//TRACE("プレイヤー %d Win\n",stage.id_ + 1);

		// 加速
		if (stage.gameOverCnt_ % 2 == 0)
		{
			speed++;
		}

		int standard = 0;
		float angle = 0;
		float angleStnd = 36.0f;

		// 文字ごとに動きを付ける
		Vector2 div{ 3,1 };
		for (int j = 0; j < div.x; j++)
		{
			int id = lpImageMng.GetHandle("win2", div, { 76,64 })[j];

			standard = static_cast<int>((maxY - winY) + (stage.size_.y / 3) - 50.0f * cosf((count + (j + 1) * 30.0f) * 0.015f));			// サインカーブ

			angle = (-DX_PI_F / angleStnd) - (DX_PI_F / (angleStnd / 2)) * cosf((count + (j + 1) * 30.0f) * 0.016f);

			lpSceneMng.AddDrawQue({ id, stage.offSet_.x + stage.blockSize_ * 2 + stage.blockSize_ * j, standard,1, angle, 0 });
			count++;
		}

		// カウントダウン中
		if (stage.gameOverCnt_-- < 0)
		{
			// 終了
			return true;
		}
		return false;
	}

private:
	int speed = -5;
	int count = 0;
	int winY = 0;
};