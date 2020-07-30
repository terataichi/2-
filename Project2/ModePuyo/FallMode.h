#pragma once
#include "../Stage.h"

struct FallMode
{
	void operator()(Stage& stage)
	{
		bool nextFlg = true;
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				// まだ動いていいかチェックをかける
				uniPuyo->SetOldDirFlg();
				nextFlg &= stage.CheckMove(uniPuyo);

				// ぷよんさせる
				if (uniPuyo->GetDirFlg().bit.down && !uniPuyo->GetOldDirFlg().bit.down)
				{
					Vector2 grid{ 0,0 };
					grid = uniPuyo->GetGrid(stage.blockSize_);

					for (int j = 0; j < (STAGE_Y - 1) - grid.y; j++)
					{
						stage.data_[grid.y + j][grid.x]->SetPuyon();
						stage.data_[grid.y + j][grid.x]->SetCnt((3 - j));
						if (j >= 3)
						{
							break;
						}
					}
				}
			});

		bool rensa = true;
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (!uniPuyo->UpDate())
				{
					// falseだったらまだ動いてるから連鎖にいかない 
					Vector2 grid = uniPuyo->GetGrid(stage.blockSize_);
					stage.data_[grid.y][grid.x].reset();
					rensa = false;
				}
			});

		if (rensa)
		{
			stage.stgMode_ = StgMode::Puyon;
		}
	}
};