#pragma once
#include "../Stage.h"

struct PuyonMode
{
	void operator()(Stage& stage)
	{

		bool flg = true;

		Vector2 grid{ 0,0 };
		for (auto vec : stage.puyoVec_)
		{
			if (vec->GetDirFlg().bit.down && !vec->GetOldDirFlg().bit.down)
			{
				grid = vec->GetGrid(stage.blockSize_);
			}
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
		}

		if (flg)
		{
			stage.stgMode_ = StgMode::Rensa;
		}
	}
};