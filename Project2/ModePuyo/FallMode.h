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
				nextFlg &= stage.CheckMove(uniPuyo);
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
				else
				{
					Vector2 grid = uniPuyo->GetGrid(stage.blockSize_);
					stage.data_[grid.y][grid.x] = uniPuyo;
				}
			});

		if (rensa)
		{
			stage.stgMode_ = StgMode::Puyon;
		}
	}
};