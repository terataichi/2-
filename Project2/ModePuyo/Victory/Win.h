#pragma once
#include "../../Stage.h"

struct Win
{
	bool operator()(Stage& stage)
	{
		//TRACE("ƒvƒŒƒCƒ„[ %d Win\n",stage.id_ + 1);
		if (stage.gameOverCnt_ < 0)
		{
			return true;
		}
		stage.gameOverCnt_--;
		return false;
	}
};