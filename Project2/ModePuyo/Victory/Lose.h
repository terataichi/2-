#pragma once
#include "../../Stage.h"
#include <cmath>

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
		return false;
	}
};