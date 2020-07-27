#pragma once
#include "../Stage.h"

struct RensaMode
{
	void operator()(Stage& stage)
	{
		bool deleteFlg = false;

		//Vector2 grid = puyoVec_[0]->GetGrid(blockSize_);
		//data_[grid.y][grid.x] = puyoVec_[0];

		for (auto&& pvec : stage.puyoVec_)
		{
			deleteFlg |= stage.SetErase(pvec, pvec->GetGrid(stage.blockSize_));
		}

		if (deleteFlg)
		{
			auto itr = std::remove_if(stage.puyoVec_.begin(), stage.puyoVec_.end(), [](std::shared_ptr<puyo>& puyo) {return !(puyo->alive()); });
			stage.puyoVec_.erase(itr, stage.puyoVec_.end());

			std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
				{
					uniPuyo->SetSpeed(1, 0);
				});
			stage.stgMode_ = StgMode::FALL;
		}
		else
		{
			stage.InstancePuyo();
			stage.CheckMove(stage.puyoVec_[0]);
			stage.stgMode_ = StgMode::Drop;
		}
	}
};