#pragma once
#include "../Stage.h"

struct PuyonMode
{
	void operator()(Stage& stage)
	{

		bool flg = true;
		auto grid = stage.puyoVec_[0]->GetGrid(stage.blockSize_);

		for (int j = 0; j < (STAGE_Y - 1) - grid.y; j++)
		{
			stage.data_[grid.y + j][grid.x]->SetPuyon();
			stage.data_[grid.y + j][grid.x]->SetWidth(5 * (3 - j));
			flg &= stage.data_[grid.y + j][grid.x]->CheckPuyon();
			if (j >= 2)
			{
				break;
			}
		}

		if (flg)
		{
			stage.stgMode_ = StgMode::Rensa;
		}
	}
};