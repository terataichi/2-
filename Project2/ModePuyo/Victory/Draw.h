#pragma once
#include "../../Stage.h"

struct Draw
{
	int speed = 0;
	bool operator()(Stage& stage)
	{
		//TRACE("ƒvƒŒƒCƒ„[ %d Draw\n", stage.id_ + 1);
		stage.gameOverPos_.y += speed;
		stage.angle_ -= atanf(0.008f);
		if (stage.gameOverCnt_ < 0)
		{
			return true;
		}
		stage.gameOverCnt_--;
		speed++;
		return false;
	}
};