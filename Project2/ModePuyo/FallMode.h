#pragma once
#include "../Stage.h"

struct FallMode
{
	void operator()(Stage& stage)
	{
		bool nextFlg = true;
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				// �܂������Ă������`�F�b�N��������
				uniPuyo->SetOldDirFlg();
				nextFlg &= stage.CheckMove(uniPuyo);

				if (uniPuyo->id() != PuyoID::Ojama)
				{
					// �Ղ�񂳂���
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
		std::for_each(stage.puyoVec_.rbegin(), stage.puyoVec_.rend(), [&](SharePuyo& uniPuyo)
			{
				if (!uniPuyo->UpDate())
				{
					// false��������܂������Ă邩��A���ɂ����Ȃ� 
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