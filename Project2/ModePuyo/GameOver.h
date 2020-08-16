#pragma once
#include "../Stage.h"

struct GameOver
{
	void operator()(Stage& stage)
	{
		TRACE("ゲームオーバー\n");
		stage.nextScene_ = stage.victoryMap_[stage.victory_](stage);
	}
};