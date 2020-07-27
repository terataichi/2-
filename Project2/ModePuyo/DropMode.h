#pragma once
#include "../Stage.h"
#include "FallMode.h"

struct DropMode
{
	void operator()(Stage& stage)
	{
		stage.modeMap_[StgMode::FALL](stage);
		stage.playUnit_->UpDate();											// 移動処理とかのUpDate
	}
};