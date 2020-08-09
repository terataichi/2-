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

				if (uniPuyo->id() != PuyoID::Ojama)
				{
					// ぷよんさせる
					if (uniPuyo->GetDirFlg().bit.down && !uniPuyo->GetOldDirFlg().bit.down)
					{
						Vector2 grid{ 0,0 };
						grid = uniPuyo->GetGrid(stage.blockSize_);

						auto max = (STAGE_Y - 1) - grid.y;
						if (max > 3)
						{
							max = 3;
						}

						for (int j = 0; j < max; j++)
						{
							if (stage.data_[grid.y + j][grid.x]->id() != PuyoID::Ojama)
							{
								stage.data_[grid.y + j][grid.x]->SetPuyon();
								stage.data_[grid.y + j][grid.x]->SetCnt((max - j));
								if (j >= 3)
								{
									break;
								}
							}
							else
							{
								break;
							}
						}
					}
				}
			});

		bool rensa = true;
		int count = 0;
		bool fall = true;
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (!uniPuyo->UpDate(count))
				{
					// falseだったらまだ動いてるから連鎖にいかない 
					Vector2 grid = uniPuyo->GetGrid(stage.blockSize_);
					stage.data_[grid.y][grid.x].reset();
					rensa = false;

					if (uniPuyo->id() == PuyoID::Ojama)
					{
						fall &= uniPuyo->seiretu();
					}
				}
				count++;
			});

		if (fall)
		{
			std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
				{
					if (uniPuyo->id() == PuyoID::Ojama)
					{
						uniPuyo->SetFall(true);
					}
				});
		}

		if (rensa)
		{
			stage.stgMode_ = StgMode::Puyon;
		}
	}
};