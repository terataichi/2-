#pragma once
#include "../Stage.h"

struct MunyonMode
{
	void operator()(Stage& stage)
	{
		bool flg = true;

		// 全員むにょん終わってるかチェック
		for (auto puyo : stage.puyoVec_)
		{
			flg &= puyo->CheckMunyon();
		}

		if (flg)
		{
			if (stage.ojamaList_.size())
			{
				int count = 0;								// 最大何個降らせるか
				for (auto ojama : stage.ojamaList_)
				{
					ojama->SetSpeed(16, 0);

					stage.puyoVec_.emplace(stage.puyoVec_.begin(), ojama);
					if (count > 30)
					{
						break;
					}
					count++;
				}
				stage.ojamaList_.clear();
				//stage.ojamaList_.remove_if(stage.ojamaList_.begin(), stage.ojamaList_.end(),
				//	[&]() {return std::next(stage.ojamaList_.begin(), count); });
				stage.stgMode_ = StgMode::FALL;
			}
			else
			{
				stage.ResetRensa();							// 連鎖のリセット
				stage.InstancePuyo();
				stage.CheckMove(stage.puyoVec_[1]);
				stage.CheckMove(stage.puyoVec_[0]);
				stage.stgMode_ = StgMode::Drop;
			}
		}
	}
};