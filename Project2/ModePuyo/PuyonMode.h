#pragma once
#include "../Stage.h"

struct PuyonMode
{
	void operator()(Stage& stage)
	{
		auto grid = stage.puyoVec_[0]->GetGrid(stage.blockSize_);

		for (int j = 0; j < 3; j++)
		{

		}

		stage.stgMode_ = StgMode::Rensa;
	}
};