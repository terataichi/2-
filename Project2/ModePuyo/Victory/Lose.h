#pragma once
#include "../../Stage.h"
#include <cmath>
#include "../../Scene/SceneMng.h"
#include "../../common/ImageMng.h"

struct Lose
{
	int speed = 10;
	bool operator()(Stage& stage)
	{
		//TRACE("ƒvƒŒƒCƒ„[ %d Lose\n", stage.id_ + 1);
		stage.gameOverPos_.y += speed;
		stage.angle_ += atanf(0.008f);
		if (stage.gameOverCnt_ < 0)
		{
			return true;
		}
		stage.gameOverCnt_--;
		speed ++;

		lpSceneMng.AddDrawQue({ lpImageMng.GetHandle("lose"), {stage.size_.x / 3 + stage.offSet_.x,stage.size_.y / 3}, 0,0 });

		return false;
	}
};