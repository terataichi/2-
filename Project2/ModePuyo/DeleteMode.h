#pragma once
#include "../Stage.h"
#include "../_debug/_DebugConOut.h"

struct DeleteMode
{
	void operator()(Stage& stage)
	{
		// 消えるときのアニメーションが最後まで終了してたらデータから消す
		// if(CheckDeathAnim)
		{
			// 消去
			auto itr = std::remove_if(stage.puyoVec_.begin(), stage.puyoVec_.end(), [](std::shared_ptr<puyo>& puyo) {return !(puyo->alive()); });
			stage.puyoVec_.erase(itr, stage.puyoVec_.end());

			// 残りのぷよのスピードを変える
			std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
				{
					uniPuyo->SetSpeed(8, 0);
					uniPuyo->UpSpeed();
				});
			stage.AddRensa();										// ここまで来てれば何か消えているので連鎖確定
			TRACE("%d連鎖\n", stage.rensa());
			stage.stgMode_ = StgMode::FALL;
		}
	}
};