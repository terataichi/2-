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
				nextFlg &= stage.CheckMove(uniPuyo);
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