#pragma once
#include "../../Stage.h"
#include <cmath>
#include "../../Scene/SceneMng.h"
#include "../../common/ImageMng.h"

struct Lose
{
	bool operator()(Stage& stage)
	{
		//TRACE("プレイヤー %d Lose\n", stage.id_ + 1);
		stage.angle_ += atanf(0.008f);

		// 一定の場所までステージを落とす
		//if (stage.gameOverPos_.y > lpSce)
		{
			stage.gameOverPos_.y += speed;
			speed++;
		}


		if (stage.gameOverCnt_ < 0)
		{
			return true;
		}
		stage.gameOverCnt_--;

		int standard = 0;
		float angle = 0;
		float angleStnd = 36.0f;

		Vector2 div{ 6,1 };
		for (int j = 0; j < div.x; j++)
		{
			int id = lpImageMng.GetHandle("lose", div, { 64,64 })[j];

			// サインカーブ				基準位置　-+　幅　*　カーブ
			standard = static_cast<int>((stage.size_.y / 3) - 50.0f * cosf((count + (j + 1) * 30) * 0.01f));
			angle = static_cast<float>((-DX_PI_F / angleStnd) - (DX_PI_F / (angleStnd / 2.0f)) * cosf((count + (j + 1) * 30.0f) * 0.01f));

			lpSceneMng.AddDrawQue({ id, {stage.offSet_.x + 32 + 64 * j, standard}, angle,0 });
			count++;
		}

		return false;
	}

private:
	int speed = 10;
	int count = 0;
};